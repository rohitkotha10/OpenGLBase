#include "Buffer.h"

namespace OpenGLBase
{
	void VertexBuffer::create()
	{
		glGenBuffers(1, &(this->data));
	}

	void VertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->data);
	}
	void VertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::setvec3(const void* verts, GLsizeiptr size)
	{
		glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
	}
	void VertexBuffer::setvec2(const void* verts, GLsizeiptr size)
	{
		glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
	}
	void VertexBuffer::erase()
	{
		glDeleteBuffers(1, &(this->data));
	}

	void IndexBuffer::create()
	{
		glGenBuffers(1, &(this->data));
	}
	void IndexBuffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->data);
	}
	void IndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void IndexBuffer::setIndices(const void* indices, GLsizeiptr size)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}
	void IndexBuffer::erase()
	{
		glDeleteBuffers(1, &(this->data));
	}
}
