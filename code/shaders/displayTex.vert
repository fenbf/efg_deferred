#version 330

// x, y, w, h of the rect
uniform vec4 rect;

layout(location = 0) in vec3 vVertex; 
layout(location = 1) in vec2 vTexCoord0;

// output: tex coord
out vec2 vVaryingTexCoord0;    

void main() 
{
    vVaryingTexCoord0 = vTexCoord0;   
    vec4 v = vec4(vVertex, 1.0);
    v.x *= rect.z;
    v.y *= rect.w;
    v.xy += rect.xy;
    gl_Position = v;         
}