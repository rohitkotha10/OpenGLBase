#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 inTex;

out vec2 TexCrd;

void main(void)
{
	gl_Position = position;
	TexCrd = inTex;
}