#pragma once

#include "header.h"

namespace QeLib {

std::string toString(const int &i);
std::string operator+(std::string const &a, const int &b);
std::string operator+(std::string const &a, const size_t &b);
std::string operator+(std::string const &a, const float &b);
std::string operator+(std::string const &a, const double &b);
std::string operator+(std::string const &a, const char *b);
std::string operator+=(std::string const &a, const int &b);
std::string operator+=(std::string const &a, const size_t &b);
std::string operator+=(std::string const &a, const float &b);
std::string operator+=(std::string const &a, const double &b);
std::string operator+=(std::string const &a, const char *b);

template <class T>
int findElementFromVector(std::vector<T> &vec, T element) {
    std::vector<T>::iterator it = std::find(vec.begin(), vec.end(), element);
    if (it == vec.end()) return INDEX_NONE;
    return int(it - vec.begin());
}

template <class T>
bool eraseElementFromVector(std::vector<T> &vec, T element) {
    int index = findElementFromVector(vec, element);
    if (index == INDEX_NONE) return false;
    vec.erase(vec.begin() + index);
    return true;
}
};  // namespace QeLib

class AeGlobalKey {
    friend class AeGlobal;

   private:
    AeGlobalKey() {}
};

class AeGlobal {
   private:
    AeGlobal();
    AeGlobalKey key;

   public:
    ~AeGlobal();

    static AeGlobal &getInstance();
    AngryEngine *engine = nullptr;
    QeVulkan *vulkan = nullptr;
    AeUI *ui = nullptr;
    QeGraphics *graphics = nullptr;
    QeCommand *command = nullptr;
    AeObjectManager *objectmanager = nullptr;
    QeScene *scene = nullptr;
};

#define GLB AeGlobal::getInstance()
#define ENGINE GLB.engine
#define VK GLB.vulkan
#define UI GLB.ui
#define GRAP GLB.graphics
#define SCENE GLB.scene
#define OBJMGR GLB.objectmanager
#define CMD(msg) GLB.command->inputCommand(msg)