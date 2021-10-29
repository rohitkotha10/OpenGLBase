#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace OpenGLBase
{
	class VertexBuffer
	{
	public:
		GLuint data;
		void create();
		void bind();
		void unbind();
		void setvec3f(const GLfloat* verts, int cnt, int destination);
		void setvec2f(const GLfloat* verts, int cnt, int destination);
		void erase();

	};

	class IndexBuffer
	{
	public:
		GLuint data;
		void create();
		void bind();
		void unbind();
		void setIndices(const GLuint* indices, int cnt);
		void erase();

	};
}
