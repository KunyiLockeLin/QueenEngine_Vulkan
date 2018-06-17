const int MAX_JOINT_NUM = 20;
const float M_PI = 3.141592653589793;
const float c_MinRoughness = 0.04;

const int layoutSet_Common = 1;
const int layoutSet_Model = 0;
const int layoutSet_Postprocessing = 2;


layout( push_constant ) uniform TimeState {
  float DeltaTime;
} pushConstant;


struct QeDataCamera {
	vec4 pos;
	mat4 view;
	mat4 projection;
};

layout( set = layoutSet_Common, binding = 0) uniform QeDataEnvironment {
	vec4 ambientColor;
	QeDataCamera camera;
	vec4 param; // 0: light num
} environmentData;


struct QeDataLight{
	vec4 pos;
	vec4 dir;
	vec4 color;
	vec4 param; // 1: type, 2: intensity, 3: coneAngle
};

layout( set = layoutSet_Common, binding = 10) buffer QeLight {
	QeDataLight lights[];
};


struct QeDataMaterial {
	vec4 baseColor;
	vec4 metallicRoughness;
	vec4 emissive;
};

layout( set = layoutSet_Model, binding = 0) uniform QeDataModel {
	mat4 model;
	mat4 joints[MAX_JOINT_NUM];
	vec4 param; // 0: billboardType / bCubemap, 1: particleFollow(2:follow 1:unfollow 0:none)
	QeDataMaterial mtl;
} modelData;


/*layout( set = layoutSet_Model, binding = 1) uniform QeDataModelViewport {
	mat4 normal;
} modelViewport;*/