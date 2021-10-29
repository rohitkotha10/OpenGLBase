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
	void VertexBuffer::setvec3f(const GLfloat* verts, int cnt, int destination)
	{
		glBufferData(GL_ARRAY_BUFFER, 3 * cnt * sizeof(float), verts, GL_STATIC_DRAW);
		glVertexAttribPointer(destination, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(destination);
	}
	void VertexBuffer::setvec2f(const GLfloat* verts, int cnt, int destination)
	{
		glBufferData(GL_ARRAY_BUFFER, 2 * cnt * sizeof(float), verts, GL_STATIC_DRAW);
		glVertexAttribPointer(destination, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(destination);
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
	void IndexBuffer::setIndices(const GLuint* indices, int cnt)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, cnt * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}
	void IndexBuffer::erase()
	{
		glDeleteBuffers(1, &(this->data));
	}
}
