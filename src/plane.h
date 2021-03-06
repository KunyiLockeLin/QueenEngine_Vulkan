#pragma once
#include "header.h"

class QePlane : public QeModel {
   public:
    COMPONENT_CLASS_DECLARE_PARENT(Plane, Model)

    // face: (0,0,1)
    bool bUpdateTargetCameraOID;

    virtual void clear();
    virtual void updatePreRender();
    virtual QeDataDescriptorSetModel createDescriptorSetModel();
};
