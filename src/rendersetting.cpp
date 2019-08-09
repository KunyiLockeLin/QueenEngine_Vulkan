#include "header.h"

void QeRenderSetting::initialize(QeAssetXML *_property, QeObject *_owner) {
    QeComponent::initialize(_property, _owner);
    GRAP->renderSetting = this;
    initProperty->getXMLbValue(&bMesh, 1, "mesh");
    initProperty->getXMLbValue(&bNormal, 1, "normal");
    initProperty->getXMLfValue(&clearColor.x, 1, "clearColorR");
    initProperty->getXMLfValue(&clearColor.y, 1, "clearColorG");
    initProperty->getXMLfValue(&clearColor.z, 1, "clearColorB");
    initProperty->getXMLfValue(&clearColor.w, 1, "clearColorA");
    initProperty->getXMLfValue(&gamma, 1, "gamma");
    initProperty->getXMLfValue(&exposure, 1, "exposure");
    initProperty->getXMLfValue(&lineWidth, 1, "lineWidth");
    initProperty->getXMLiValue(&msaa, 1, "msaa");

    VkSampleCountFlags counts = std::min(VK->deviceProperties.limits.framebufferColorSampleCounts,
                                          VK->deviceProperties.limits.framebufferDepthSampleCounts);

    if (counts > uint32_t(msaa)) counts = msaa;
    if (counts >= VK_SAMPLE_COUNT_64_BIT)
        sampleCount = VK_SAMPLE_COUNT_64_BIT;
    else if (counts >= VK_SAMPLE_COUNT_32_BIT)
        sampleCount = VK_SAMPLE_COUNT_32_BIT;
    else if (counts >= VK_SAMPLE_COUNT_16_BIT)
        sampleCount = VK_SAMPLE_COUNT_16_BIT;
    else if (counts >= VK_SAMPLE_COUNT_8_BIT)
        sampleCount = VK_SAMPLE_COUNT_8_BIT;
    else if (counts >= VK_SAMPLE_COUNT_4_BIT)
        sampleCount = VK_SAMPLE_COUNT_4_BIT;
    else if (counts >= VK_SAMPLE_COUNT_2_BIT)
        sampleCount = VK_SAMPLE_COUNT_2_BIT;
    else
        sampleCount = VK_SAMPLE_COUNT_1_BIT;
}
