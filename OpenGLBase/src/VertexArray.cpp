#include "VertexArray.h"

namespace OpenGLBase\
{
	void VertexArray::create()
	{
		return glGenVertexArrays(1, &(this->data));
	}

	void VertexArray::bind()
	{
		return glBindVertexArray(this->data);
	}

	void VertexArray::unbind()
	{
		return glBindVertexArray(0);
	}

	void VertexArray::erase()
	{
		return glDeleteVertexArrays(1, &(this->data));
	}
}
