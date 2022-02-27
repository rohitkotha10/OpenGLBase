#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>
#include <string>

namespace OpenGLBase
{
	class TexBuffer
	{
		GLuint id;
		unsigned char* texSource;
		int width, height, nrChannels;
	public:
		GLuint getId() { return id; };
		void create();
		void bind();
		void unbind();
		void setTexture(std::string path);
		void active(int destination);
		void erase();
	};
}
