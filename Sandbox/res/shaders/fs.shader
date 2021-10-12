#version 450 core

out vec4 color;

in vec2 TexCrd;

uniform sampler2D myTex1;
uniform sampler2D myTex2;

void main(void)
{
	color = mix(texture(myTex1, TexCrd), texture(myTex2, TexCrd), 0.2);
}