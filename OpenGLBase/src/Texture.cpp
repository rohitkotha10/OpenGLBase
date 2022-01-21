#include "Texture.h"
#include <iostream>

namespace OpenGLBase
{
	void Texture::create()
	{
		glGenTextures(1, &(this->data));
	}
	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, this->data);
	}
	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setTexture(std::string path)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		this->texSource = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (!(this->texSource))
			std::cout << "Fail Texture Load" << std::endl;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->texSource);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::activate(TexDestination destination)
	{
		this->bind();
		glActiveTexture(destination);
	}
	void Texture::erase()
	{
		glDeleteTextures(1, &(this->data));
	}

	void flipTexture(bool flag)
	{
		stbi_set_flip_vertically_on_load(flag);
	}
}
