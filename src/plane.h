#pragma once
#include "header.h"

class QePlane : public QeModel {
   public:
    // face: (0,0,1)

    QePlane(AeObjectManagerKey &_key) : QeModel(_key) {}
    //~QePlane() {}
    QePlaneType planeType;
    int targetCameraOID;
    bool bUpdateTargetCameraOID;

    virtual void initialize(QeAssetXML *_property, QeObject *_owner);
    virtual void clear();
    virtual void update1();
    virtual QeDataDescriptorSetModel createDescriptorSetModel();
};