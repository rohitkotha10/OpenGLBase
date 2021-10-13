#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 inTex;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec2 TexCrd;

void main(void)
{
	gl_Position = proj_matrix * mv_matrix * position;
	TexCrd = inTex;
}
