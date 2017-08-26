#pragma once

#include "qeheader.h"


struct QeDataMVP {
	QeMatrix4x4f model;
	QeMatrix4x4f view;
	QeMatrix4x4f proj;
	QeMatrix4x4f normal;
};

class QeModel
{
public:

	QeVector3f pos;
	float face;
	float up;
	QeVector3f size;

	QeAssetModel* modelData;

	VkDescriptorSet descriptorSet;
	VkBuffer mvpBuffer;
	VkDeviceMemory mvpBufferMemory;
	VkBuffer lightBuffer;
	VkDeviceMemory lightBufferMemory;

	VkPipeline graphicsPipeline;
	VkDescriptorPool descriptorPool;

	void createDescriptorBuffer();
	void updateUniformBuffer();
	void update(float time);
	
	QeModel(QeObjectMangerKey& _key) {}
	~QeModel() {}

	void init(const char* _filename);
	void cleanup();
	void setPosFaceUpSize(QeVector3f _pos, float _face, float _up, QeVector3f _size);
	void move(QeVector3f _pos);
	void setPosition(QeVector3f _pos);
	void rotateFace( float _face);
	void setFace(float _face);
	void rotateUp(float _up);
	void setUp(float _up);
	void enlarge(QeVector3f _size);
	void setSize(QeVector3f _size);
	QeMatrix4x4f getMatModel();
	void cleanupSwapChain();
	void recreateSwapChain();
};

