#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (input_attachment_index=0, set=0, binding=5) uniform subpassInput inputAttachment;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

void main() 
{
	outColor = subpassLoad(inputAttachment);
	outColor.r = (outColor.r+outColor.g +outColor.b)/3; 
	outColor.g = outColor.r;
	outColor.b = outColor.r; 
}