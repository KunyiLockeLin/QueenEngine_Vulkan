#include "header.h"

void QeCubemap::initialize(AeXMLNode *_property, QeObject *_owner) {
    COMPONENT_INITIALIZE

    modelData = G_AST.getModel("cube", true);
    materialData = G_AST.getMaterialImage(component_data.image.c_str(), true);

    VK->createBuffer(modelBuffer, sizeof(bufferData), nullptr);
    bufferData.material = materialData->value;
    shaderKey = "cubemap";
    G_AST.setGraphicsShader(graphicsShader, nullptr, shaderKey);
    graphicsPipeline.bAlpha = false;
    GRAP->models.push_back(this);

    VK->createDescriptorSet(descriptorSet);
    VK->updateDescriptorSet(&createDescriptorSetModel(), descriptorSet);
}

/*QeDataDescriptorSetModel QeCubemap::createDescriptorSetModel() {
        QeDataDescriptorSetModel descriptorSetData;
        descriptorSetData.modelBuffer = modelBuffer.buffer;
        bufferData.param = { 0,0,0 };
        descriptorSetData.cubeMapImageView = materialData->image.pCubeMap->view;
        descriptorSetData.cubeMapSampler =
materialData->image.pCubeMap->sampler; bufferData.param.z = 1;

        return descriptorSetData;
}*/
