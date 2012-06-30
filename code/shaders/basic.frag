#version 330

in vec2 vVaryingTexCoord0;
out vec4 vFragColor;

uniform sampler2D texture0;

void main()
{
    vFragColor = texture(texture0, vVaryingTexCoord0);
}