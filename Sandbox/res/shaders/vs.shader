#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normalIn;
layout (location = 2) in vec2 texIn;

out vec4 normalOut;
out vec4 fragPos;
out vec2 texOut;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main(void)
{
	gl_Position = proj_matrix * view_matrix * model_matrix * position;
	fragPos = model_matrix * position;
	normalOut = mat4(transpose(inverse(model_matrix))) * normalIn;
	texOut = texIn;
}
