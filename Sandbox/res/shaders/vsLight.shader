#version 450 core

layout (location = 0) in vec4 position;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main(void)
{
	gl_Position = proj_matrix * view_matrix * model_matrix * position;
}
