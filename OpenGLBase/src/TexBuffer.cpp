#include "TexBuffer.h"
#include <iostream>

namespace OpenGLBase
{
	void TexBuffer::create()
	{
		glGenTextures(1, &(this->id));
	}
	void TexBuffer::bind()
	{
		glBindTexture(GL_TEXTURE_2D, this->id);
	}
	void TexBuffer::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TexBuffer::setTexture(std::string path)
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
		stbi_image_free(this->texSource);
	}

	void TexBuffer::active(int destination)
	{
		glActiveTexture(GL_TEXTURE0 + destination);
		this->bind();
	}
	void TexBuffer::erase()
	{
		glDeleteTextures(1, &(this->id));
	}
}
