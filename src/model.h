#pragma once

#include "header.h"

struct QeDataModel {
    QeMatrix4x4f model;
    QeMatrix4x4f joints[MAX_JOINT_NUM];
    AeArray<float,4> param1;  // 0: bColorMap, 1: bCubeMap, 2: bNormalMap, 3:
                        // bMetallicRoughnessMap
    AeArray<float, 4> param2;  // 0: outlineWidth, 1: vertexNum
    QeDataMaterial material;
};

class QeModel : public QeComponent {
   public:
    COMPONENT_CLASS_DECLARE_INITIALIZE(Model, modelBuffer(eBuffer_uniform), descriptorSet(eDescriptorSetLayout_Model))

    virtual void clear();
    virtual void updatePreRender();

    // QeMaterial, QeAnimation
    const char *shaderKey;
    QeAssetModel *modelData = nullptr;
    QeAssetMaterial *materialData = nullptr;
    QeAssetGraphicsShader graphicsShader;
    QeAssetGraphicsShader normalShader;
    QeAssetGraphicsShader outlineShader;
    QeDataGraphicsPipeline graphicsPipeline;

    QeDataDescriptorSet descriptorSet;

    QeDataModel bufferData;
    QeVKBuffer modelBuffer;
    bool bUpdateMaterialOID;
    bool bRotate = true;
    bool b2D = false;

    virtual QeDataDescriptorSetModel createDescriptorSetModel();
    virtual bool isShowByCulling(QeCamera *camera);

    virtual void updateDrawCommandBuffer(QeDataDrawCommand *command);
    virtual void updateComputeCommandBuffer(VkCommandBuffer &commandBuffer) {}
};
