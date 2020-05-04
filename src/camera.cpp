#include "header.h"

void QeCamera::initialize(QeAssetXML *_property, QeObject *_owner) {
    QeComponent::initialize(_property, _owner);
    // type = eCameraThirdPerson;
    bufferData.pos_rayTracingDepth.w = initProperty->getXMLValuef("raytracingDepth");
    lookAtTransformOID = initProperty->getXMLValuei("lookAtTransformOID");
    up.x = initProperty->getXMLValuef("upX");
    up.y = initProperty->getXMLValuef("upY");
    up.z = initProperty->getXMLValuef("upZ");
    aperture = initProperty->getXMLValuef("aperture");
    bufferData.vertical_lensRadius.w = aperture / 2;

    speed = initProperty->getXMLValuef("speed");
    cullingDistance = initProperty->getXMLValuei("cullingDistance");
    fov = initProperty->getXMLValuef("fov");
    fnear = initProperty->getXMLValuef("near");
    ffar = initProperty->getXMLValuef("far");
    postProcessingOID = initProperty->getXMLValuei("postProcessingOID");
    bUpdatePostProcessingOID = false;
    if (postProcessingOID) bUpdatePostProcessingOID = true;

    renderType = (QeRenderType)initProperty->getXMLValuei("renderType");
    renderSize.width = initProperty->getXMLValuei("width");
    renderSize.height = initProperty->getXMLValuei("height");
    GRAP->createRender(renderType, oid, renderSize);
}

void QeCamera::rotateTarget(QeVector3f _addRotate) {
    if (renderType == eRender_ui) return;

    QeVector3f lookAtV = lookAt();
    QeVector3f pos = owner->transform->worldPosition();
    QeVector3f vec = {pos - lookAtV};

    QeVector3f euler = MATH->vectorToEulerAngles(vec);
    if ((euler.y < -85 && _addRotate.y < 0) || (euler.y > 85 && _addRotate.y > 0)) return;
    _addRotate *= speed;

    if (_addRotate.x) {
        float f = euler.y + _addRotate.y;
        if (f < -85)
            _addRotate.y = -89 - euler.y;
        else if (f > 85)
            _addRotate.y = 89 - euler.y;
    }
    owner->transform->revolute(_addRotate, lookAtV, false, false, true);
}

void QeCamera::rotateTargetByMouse(QeVector2i mousePos) {
    rotateTarget({0.f, float(mousePos.y - lastMousePos.y), float(mousePos.x - lastMousePos.x)});
    lastMousePos = mousePos;
}

void QeCamera::setMousePos(QeVector2i mousePos) { lastMousePos = mousePos; }

void QeCamera::zoomInOut(QeVector2i mousePos) {
    move({0, 0, -(mousePos.x - lastMousePos.x) / 10}, false);
    lastMousePos = mousePos;
}

void QeCamera::move(QeVector3f _dir, bool bMoveCenter) {
    QeVector3f lookAtV = lookAt();
    QeVector3f pos = owner->transform->worldPosition();
    QeVector3f face = pos - lookAtV;

    if (renderType == eRender_ui) {
        _dir *= speed;
        pos += _dir;
        owner->transform->setWorldPosition(pos);
        return;
    }

    // forward
    if (_dir.z > 0 && !bMoveCenter && MATH->length(face) < 1) {
        return;
    }
    _dir *= speed;

    owner->transform->move(_dir, face, up);
    pos = owner->transform->worldPosition();

    if (bMoveCenter) {
        QeTransform *lookAtTransform = (QeTransform *)OBJMGR->findComponent(eComponent_transform, lookAtTransformOID);
        lookAtTransform->move(_dir, face, up);
        owner->transform->setWorldPosition(pos);
    }
}

bool QeCamera::isRaytracing() { return bufferData.pos_rayTracingDepth.w > 0 ? true : false; }

QeVector3f QeCamera::face() {
    if (renderType == eRender_ui) return {0, 0, 1};
    return MATH->normalize(lookAt() - owner->transform->worldPosition());
}

QeVector3f QeCamera::lookAt() {
    if (renderType == eRender_ui) {
        QeVector3f pos = owner->transform->worldPosition();
        return {pos.x, pos.y, pos.z + 1};
    }
    if (lookAtTransformOID > 0) {
        QeTransform *lookAtTransform = (QeTransform *)OBJMGR->findComponent(eComponent_transform, lookAtTransformOID);
        return lookAtTransform->worldPosition();
    }
    return {0.f, 0.f, 0.f};
}

void QeCamera::reset() {
    if (lookAtTransformOID > 0) {
        QeTransform *lookAtTransform = (QeTransform *)OBJMGR->findComponent(eComponent_transform, lookAtTransformOID);
        lookAtTransform->initialize(lookAtTransform->owner->transform->initProperty, lookAtTransform->owner);
    }
    owner->transform->initialize(owner->transform->initProperty, owner);
    // initialize(initProperty, owner);
}

void QeCamera::setLookAtTransformOID(int _lookAtransformOID) {
    if (!_lookAtransformOID) return;

    QeTransform *lookAtTransform1 = (QeTransform *)OBJMGR->findComponent(eComponent_transform, _lookAtransformOID);
    if (!lookAtTransform1) return;
    QeTransform *lookAtTransform2 = (QeTransform *)OBJMGR->findComponent(eComponent_transform, lookAtTransformOID);
    if (!lookAtTransform2) return;

    lookAtTransform2->setWorldPosition(lookAtTransform1->worldPosition());
    owner->transform->position = {20.f, 0.f, 1.f};
}

void QeCamera::update1() {
    if (bUpdatePostProcessingOID && postProcessingOID) {
        if (GRAP->addPostProcssing(renderType, oid, postProcessingOID)) {
            bUpdatePostProcessingOID = false;
        }
    }

    QeVector3f pos = owner->transform->worldPosition();
    bufferData.pos_rayTracingDepth = pos;
    QeVector3f lookAtV = lookAt();
    QeVector3f face = lookAtV - pos;
    float focusDist = MATH->length(face);
    face = MATH->normalize(face);
    // focusDist = 1;
    if (renderType == eRender_ui) {
        bufferData.horizontal_aspect.w = ((float)renderSize.width) / renderSize.height;
    } else {
        bufferData.view = MATH->lookAt(pos, lookAtV, up);
        bufferData.projection = MATH->perspective(fov, bufferData.horizontal_aspect.w, fnear, ffar);

        float half_height = -1.0f / bufferData.projection._11;            // tan(theta/2);
        float half_width = bufferData.horizontal_aspect.w * half_height;  // 1.0/cam.projection[0][0];

        QeVector3f w = face;  // vec3(cam.view[0][2], cam.view[1][2], cam.view[2][2]);
        QeVector3f u = QeVector3f(bufferData.view._00, bufferData.view._10,
                                  bufferData.view._20);  // cross(vup, w);
        QeVector3f v = QeVector3f(bufferData.view._01, bufferData.view._11,
                                  bufferData.view._21);  // cross(w, u);
        bufferData.lowerLeftCorner = pos - u * half_width * focusDist - v * half_height * focusDist + w * focusDist;
        bufferData.horizontal_aspect = u * 2 * half_width * focusDist;
        bufferData.vertical_lensRadius = v * 2 * half_height * focusDist;
    }
}