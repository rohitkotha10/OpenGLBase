#version 450 core

in vec3 normalOut;
in vec3 fragPos;
in vec2 texOut;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightPos;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light 
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform Material material;


void main(void)
{
	vec3 ambient = vec3(texture(material.diffuse, texOut)) * light.ambient;

	vec3 norm = normalize(normalOut);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, texOut)) * light.diffuse;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, texOut)) * spec * light.specular;

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}