#include "qeheader.h"

void QeAnimation::initialize(QeAssetXML* _property, QeObject* _owner) {
	QeModel::initialize(_property, _owner);

	AST->getXMLiValue((int*)&actionState, initProperty, 1, "actionState");
	AST->getXMLfValue(&actionSpeed, initProperty, 1, "actionSpeed");
	AST->getXMLiValue(&currentActionID, initProperty, 1, "actionID");
	AST->getXMLiValue((int*)&actionType, initProperty, 1, "actionType");

	graphicsShader.geom = nullptr;
	graphicsShader.tesc = nullptr;
	graphicsShader.tese = nullptr;
	graphicsShader.vert = nullptr;
	graphicsShader.frag = nullptr;
	AST->setGraphicsShader(graphicsShader, initProperty, "action");
	currentActionFrame=0;
	currentActionTime=0;
}

void QeAnimation::update1() {
	updateAction();
	QeModel::update1();
}

bool QeAnimation::setAction(unsigned int actionID, QeActionType type) {
	if (actionID <= modelData->animationNum) {
		currentActionID = actionID;
		actionType = type;
		return true;
	}
	return false;
}

void QeAnimation::actionPlay() {

	currentActionFrame = modelData->animationStartFrames[currentActionID];
	currentActionTime = modelData->jointsAnimation[0].rotationInput[currentActionFrame];
	actionState = eActionStatePlay;
}

void QeAnimation::actionPause() { actionState = eActionStatePause; }
void QeAnimation::actionStop() { actionState = eActionStateStop; }

void QeAnimation::updateAction() {
	if (!modelData || !modelData->rootJoint || (actionState != eActionStatePlay && QE->deltaTime)) return;

	float previousActionFrameTime = modelData->jointsAnimation[0].rotationInput[currentActionFrame];
	float nextActionFrameTime = modelData->jointsAnimation[0].rotationInput[currentActionFrame + 1];
	bool bFinalFrame = false;
	if ((currentActionFrame + 1) == modelData->animationEndFrames[currentActionID]) bFinalFrame = true;

	float progessive = MATH->clamp((currentActionTime - previousActionFrameTime) / (nextActionFrameTime - previousActionFrameTime), 0.f, 1.f);

	QeVector3f previousTranslation, nextTranslation, currentTranslation, currentScale;
	QeVector4f previousRotation, nextRotation, currentRotation;
	currentScale = { 1,1,1 };

	size_t size = modelData->jointsAnimation.size();

	for (size_t i = 0; i<size; ++i) {
		previousTranslation = modelData->jointsAnimation[i].translationOutput[currentActionFrame];
		nextTranslation = modelData->jointsAnimation[i].translationOutput[currentActionFrame + 1];
		currentTranslation = MATH->interpolatePos(previousTranslation, nextTranslation, progessive);

		previousRotation = modelData->jointsAnimation[i].rotationOutput[currentActionFrame];
		nextRotation = modelData->jointsAnimation[i].rotationOutput[currentActionFrame + 1];
		currentRotation = MATH->interpolateDir(previousRotation, nextRotation, progessive);
		joints[i] = MATH->transform(currentTranslation, currentRotation, currentScale);
	}

	QeMatrix4x4f mat;
	setChildrenJointTransform(*modelData->rootJoint, mat);

	currentActionTime += QE->deltaTime * actionSpeed;
	if (currentActionTime > nextActionFrameTime) {

		if (bFinalFrame) {
			if (actionType == eActionTypeOnce) actionStop();
			else {
				if (actionType == eActionTypeNext) {
					++currentActionID;
					if (currentActionID >= modelData->animationNum) currentActionID -= modelData->animationNum;
				}
				actionPlay();
			}
		}
		else	++currentActionFrame;
	}
}

void QeAnimation::setChildrenJointTransform(QeDataJoint& joint, QeMatrix4x4f &parentTransform) {

	size_t size = modelData->jointsAnimation.size();
	QeVector3f scale(1, 1, 1);

	for (size_t i = 0; i<size; ++i) {
		if (modelData->jointsAnimation[i].id == joint.id) {

			joints[i] = parentTransform * joints[i];

			size_t size1 = modelData->jointsAnimation[i].children.size();
			for (size_t j = 0; j<size1; ++j)
				setChildrenJointTransform(*modelData->jointsAnimation[i].children[j], joints[i]);

			bufferData.joints[i] = joints[i] * modelData->jointsAnimation[i].inverseBindMatrix;
			break;
		}
	}
}