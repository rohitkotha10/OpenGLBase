#version 450 core

in vec3 normalOut;
in vec3 fragPos;
in vec2 texOut;

out vec4 fragColor;

struct Material
{
	sampler2D diffuse1;
	sampler2D diffuse2;
	sampler2D diffuse3;
	sampler2D specular1;
	sampler2D specular2;
	sampler2D specular3;
	float shininess;
};

struct Light
{
	vec3 position;

	float ambient;
	float diffuse;
	float specular;
};

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

void main(void)
{
	vec3 ambient = vec3(texture(material.diffuse1, texOut)) * light.ambient;

	vec3 norm = normalize(normalOut);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse1, texOut)) * light.diffuse;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular1, texOut)) * spec * light.specular;

	vec3 result = ambient + diffuse + specular;

	fragColor = vec4(result, 1.0);
}