#version 450 core

in vec4 normalOut;
in vec4 fragPos;

out vec4 fragColor;

uniform vec4 objectColor;
uniform vec4 lightColor;
uniform vec4 viewPos;
uniform vec4 lightPos;

void main(void)
{
	float ambientStrength = 0.1;
	vec4 ambient = ambientStrength * lightColor;

	vec4 norm = normalize(normalOut);
	vec4 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec4 viewDir = normalize(viewPos - fragPos);
	vec4 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.2);
	vec4 specular = specularStrength * spec * lightColor;

	vec4 result = (ambient + diffuse + specular) * objectColor;
	fragColor = result;
}