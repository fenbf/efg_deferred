#version 330

uniform sampler2D texture0;

in vec3 vVaryingPos;
in vec3 vVaryingNormal;
in vec2 vVaryingTexCoord0; 

layout(location = 0) out vec3 vFragPos;
layout(location = 1) out vec4 vFragNormal;
layout(location = 2) out vec4 vFragColor;

void main()
{
	vFragPos = vVaryingPos;
	vFragNormal.xyz = (vVaryingNormal+vec3(1.0))*0.5;
	vFragNormal.w = 0.0;

	vFragColor = texture(texture0, vVaryingTexCoord0);
}