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

	void VertexArray::erase()
	{
		glDeleteVertexArrays(1, &(this->data));
	}
}
