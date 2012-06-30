#version 330

uniform sampler2D tex;

in vec2 vVaryingTexCoord0;

out vec4 vFragColor;

void main()
{
	vFragColor = texture(tex, vVaryingTexCoord0.xy);
}