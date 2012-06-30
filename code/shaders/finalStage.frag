#version 330

//uniform sampler2D texDepth;
uniform sampler2D texPos;
uniform sampler2D texNormal;
uniform sampler2D texColor;

uniform vec3 lightPos;
uniform vec3 lightDiffuse;
uniform vec4 lightSpecular;

uniform float nearPlane;
uniform float farPlane;
uniform float viewWidth;
uniform float viewHeight;
uniform vec2 texelSize;

in vec2 vVaryingTexCoord0;

out vec4 vFragColor;

void main()
{
	//float depth  = texture(texDepth, vVaryingTexCoord0.xy).r;
	vec3 norm = texture(texNormal, vVaryingTexCoord0.xy).rgb; 
	vec3 normal  = norm*2.0 - vec3(1.0); 
	normal.z = sqrt(1.0 - normal.x*normal.x - normal.y*normal.y);
	vec4 color   = texture(texColor, vVaryingTexCoord0.xy).rgba; 
 
	vec3 position = texture(texPos, vVaryingTexCoord0.xy).rgb;
	//position.z = -nearPlane / (farPlane - (depth * (farPlane - nearPlane))) * farPlane;
	//position.x = ((gl_FragCoord.x / viewWidth) * 2.0) - 1.0;
	//position.y = (((gl_FragCoord.y / viewHeight) * 2.0) - 1.0) * (viewHeight / viewWidth);
	//position.x *= position.z;
	//position.y *= position.z;
 
	float dist = 2.0f/distance(position, lightPos);
 
	//
	// lighting
	//
	vec3 lightVector = normalize(lightPos - position);	
	vec3 halfVector  = normalize(lightVector + normalize(-position));
	float nl = dot(lightVector, normal);
	vec3 diffuse = lightDiffuse * max(0.0, nl);
	vec3 specular = vec3(0.0);
	if (nl > 0.0)
	    specular = lightSpecular.xyz * pow(max(0.0, dot(halfVector, normal)), lightSpecular.w);
	
 
	vFragColor.rgb = 1.0*(color.rgb*diffuse*dist + specular*dist);
	vFragColor.a = 1.0f;
}