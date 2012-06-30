#version 330

layout(location = 0) in vec3 vVertex; 
layout(location = 1) in vec2 vTexCoord0;

// output: tex coord
out vec2 vVaryingTexCoord0;    

void main() 
{
    vVaryingTexCoord0 = vTexCoord0;    	
    gl_Position = vec4(vVertex, 1.0);         
}