#version 450 core

in vec3 normalOut;
in vec3 fragPos;
in vec2 texOut;

out vec4 fragColor;

struct Material
{
	sampler2D diffuse1;
	sampler2D specular;
	float shininess;
};

uniform Material material;

void main(void)
{
	fragColor = texture(material.diffuse1, texOut);
}