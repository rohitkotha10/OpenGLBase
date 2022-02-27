#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace OpenGLBase
{
	class VertexBuffer
	{
	public:
		GLuint id;
		void create();
		void bind();
		void unbind();
		void setvec3(const void* verts, GLsizeiptr size);
		void setvec2(const void* verts, GLsizeiptr size);
		void erase();

	};

	class IndexBuffer
	{
	public:
		GLuint id;
		void create();
		void bind();
		void unbind();
		void setIndices(const void* indices, GLsizeiptr size);
		void erase();

	};
}
