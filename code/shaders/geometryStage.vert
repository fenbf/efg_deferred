#version 330

uniform mat4 modelviewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;  
layout(location = 2) in vec2 vTexCoord0;

out vec3 vVaryingPos;
out vec3 vVaryingNormal;
out vec2 vVaryingTexCoord0;       

void main() 
{
    vVaryingTexCoord0 = vTexCoord0;      
    
	vec4 v = vec4(vVertex, 1.0);  
	
	vVaryingNormal = normalize(normalMatrix * vNormal);
	vVaryingPos = modelviewMatrix * v;
    gl_Position = projectionMatrix * modelviewMatrix * v;         
}