#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>
#include <string>

namespace OpenGLBase
{
	void flipTexture(bool flag);

	enum TextureType
	{
		TEX2D = GL_TEXTURE_2D
	};

	enum TexDestination
	{
		TEX0 = GL_TEXTURE0, TEX1 = GL_TEXTURE1, TEX2 = GL_TEXTURE2, TEX3 = GL_TEXTURE3
	};

	class Texture
	{
		GLuint data;
		unsigned char* texSource;
		int width, height, nrChannels;
	public:
		GLuint getData() { return data; };
		void create();
		void bind();
		void unbind();
		void setTexture(std::string path);
		void activate(TexDestination destination);
		void erase();
	};
}
