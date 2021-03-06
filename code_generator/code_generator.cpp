#pragma once

#include <stdio.h>
#include "common/common.h"
#include <direct.h>
//#include "src/generated_config_struct_enum.h"

const std::string indent = "    ";
const std::string gameobject_struct_name = "AeGameObjectData";
AeXMLNode* config = nullptr;

struct enum_data {
    std::string key;
    std::string name;
    std::string prefix;
    std::vector<AeNode> enums;
};

std::string AddElementCode(std::string& key, std::vector<std::string>& types, std::vector<std::string>& load_codes) {
    if (types.size() != 2) {
        load_codes.push_back(key + " = property_->getXMLValue<" + types[0] + ">(\"" + key + "\");");
        return (types[0] + " " + key + ";");
    }
    std::string type1 = "AeArray<" + types[0] + ", " + types[1] + ">";
    load_codes.push_back(key + " = property_->getXMLValues<" + types[0] + ", " + types[1] + ">(\"" + key + "\");");
    return (type1 + " " + key + ";");
}

void AddGameObjectComponentStrcut(AeXMLNode& node, AeFile& file) {
    // struct
    file.addNewLine("");
    std::string s = "struct ";
    s += (gameobject_struct_name + "Component" + node.data->key + " {");
    file.addNewLine(s.c_str());
    auto* define_node = node.getXMLNode("define");
    std::vector<std::string> load_codes;

    for (auto& element : define_node->data->elements) {
        s = indent;
        std::vector<std::string> types = ENCODE.split<std::string>(element.value, " ");
        if (types[0].compare("enum") == 0) {
            std::string key = "enum_define." + element.key + ".name";
            std::string enum_name = config->getXMLValue<std::string>(key.c_str());
            s += (enum_name + " " + element.key + ";");
            load_codes.push_back(element.key + " = static_cast<" + enum_name + ">(property_->getXMLValue<int>(\"" +
                                    element.key + "\"));");
        } else {
            s += AddElementCode(element.key, types, load_codes);
        }
        file.addNewLine(s.c_str());
    }

    // read xml
    file.addNewLine("");
    file.addNewLine((indent + "AeXMLNode* property_;").c_str());
    file.addNewLine((indent + "void read(AeXMLNode& node) {").c_str());
    file.addNewLine((indent + indent + "property_ = &node;").c_str());
    for (auto& load_code : load_codes) {
        file.addNewLine((indent + indent + load_code).c_str());
    }
    file.addNewLine((indent + "}").c_str());

    file.addNewLine((indent + "void reset() { read(*property_); }").c_str());
    file.addNewLine("};");
}

int main(int argc, char* argv[]) {
    // generated_config_struct_enum.h
    LOG("Create generated\\generated_config_struct_enum.h");
    AeFile file;
    file.open("..\\..\\generated\\generated_config_struct_enum.h");
    file.addNewLine("#pragma once");
    file.addNewLine("//This file is generated by code_generator.cpp.DO NOT edit this file.");
    file.addNewLine("#include \"common/common.h\"");
    // file.addNewLine("#include <cstring>");

    config = CM_MGR.getXML("..\\..\\output\\data\\config.xml");

    // enum
    auto* enum_define = config->getXMLNode("enum_define");
    std::vector<enum_data> enum_datas;
    for (auto node : enum_define->data->nexts) {
        enum_data e_data;
        e_data.key = node->data->key;

        for (auto e : node->data->elements) {
            if (e.key.compare("name") == 0) {
                e_data.name = e.value;
            } else if (e.key.compare("prefix") == 0) {
                e_data.prefix = e.value;
            } else {
                e_data.enums.push_back(e);
            }
        }

        if (e_data.key.compare("type") == 0) {
            auto* node = config->getXMLNode("scenes.default");
            e_data.enums.push_back({"Scene", node->getXMLValue<const char*>("type")});

            node = config->getXMLNode("objects.default");
            e_data.enums.push_back({"Object", node->getXMLValue<const char*>("type")});

            node = config->getXMLNode("components");
            std::string key = "Component_";
            for (auto comp : node->data->nexts) {
                auto* node2 = comp->getXMLNode("default");
                e_data.enums.push_back({key + comp->data->key, node2->getXMLValue<const char*>("type")});
            }
        }
        enum_datas.push_back(e_data);
    }
    for (auto e_data : enum_datas) {
        file.addNewLine("");

        std::string s = "enum ";
        s += (e_data.name + " {");
        file.addNewLine(s.c_str());
        for (auto e : e_data.enums) {
            std::string s = indent + e_data.prefix + e.key + " = " + e.value + ",";
            file.addNewLine(s.c_str());
        }
        file.addNewLine("};");

        file.addNewLine("");
        s = "static inline const char* string_";
        s += e_data.name + "(" + e_data.name + " type) {";
        file.addNewLine(s.c_str());
        file.addNewLine((indent + "switch (type) {").c_str());
        for (auto e : e_data.enums) {
            file.addNewLine((indent + indent + "CASE_STR(" + e_data.prefix + e.key + ");").c_str());
        }
        file.addNewLine((indent + indent + "default:").c_str());
        file.addNewLine((indent + indent + indent + "ASSERT(0,\"" + e_data.prefix + "INVALID\");").c_str());
        file.addNewLine((indent + "}").c_str());
        //file.addNewLine((indent + "return \"" + e_data.prefix + "INVALID\";").c_str());
        file.addNewLine("}");
    }

    // struct
    file.addNewLine("");
    std::string type_name = enum_define->getXMLValue<std::string>("type.name");
    file.addNewLine("");
    std::string s = "struct ";
    s += (gameobject_struct_name + " {");
    file.addNewLine(s.c_str());
    file.addNewLine((indent + "std::string name;").c_str());
    file.addNewLine((indent + type_name + " type;").c_str());
    file.addNewLine((indent + "int oid;").c_str());
    file.addNewLine((indent + "int eid;").c_str());
    file.addNewLine("");
    file.addNewLine((indent + "AeXMLNode* property_;").c_str());
    file.addNewLine((indent + "void read(AeXMLNode& node) {").c_str());
    file.addNewLine((indent + indent + "property_ = &node;").c_str());
    file.addNewLine((indent + indent + "name = property_->data->key;").c_str());
    file.addNewLine((indent + indent + "type = static_cast<" + type_name + ">(property_->getXMLValue<int>(\"type\"));").c_str());
    file.addNewLine((indent + indent + "oid = property_->getXMLValue<int>(\"oid\");").c_str());
    file.addNewLine((indent + indent + "eid = property_->getXMLValue<int>(\"eid\");").c_str());
    file.addNewLine((indent + "}").c_str());
    file.addNewLine((indent + "void reset() { read(*property_); }").c_str());
    file.addNewLine("};");

    auto node = config->getXMLNode("components");
    for (auto comp : node->data->nexts) {
        AddGameObjectComponentStrcut(*comp, file);
    }

    file.close();
    LOG("CodeGenerator End.");
    return 0;
}
