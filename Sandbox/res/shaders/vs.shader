#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texIn;

out vec3 normalOut;
out vec3 fragPos;
out vec2 texOut;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main(void)
{
	gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);
	fragPos = vec3(model_matrix * vec4(position, 1.0));
	normalOut = mat3(transpose(inverse(model_matrix))) * normalIn;
	texOut = texIn;
}
