#include "VertexArray.h"

namespace OpenGLBase
{
	void VertexArray::create()
	{
		glGenVertexArrays(1, &(this->data));
	}

	void VertexArray::bind()
	{
		glBindVertexArray(this->data);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::setAttrib(GLuint destination, GLint size, GLenum type)
	{
		glVertexAttribPointer(destination, size, type, GL_FALSE, 0, NULL);
	}

	void VertexArray::enable(GLuint destination)
	{
		glEnableVertexAttribArray(destination);
	}

	void VertexArray::erase()
	{
		glDeleteVertexArrays(1, &(this->data));
	}
}
