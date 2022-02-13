#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>
#include <string>

namespace OpenGLBase
{
	class TexBuffer
	{
		GLuint data;
		unsigned char* texSource;
		int width, height, nrChannels;
	public:
		GLuint getData() { return data; };
		void create();
		void bind();
		void unbind();
		void setTexture(std::string path, bool flip);
		void active(GLenum destination);
		void erase();
	};
}
