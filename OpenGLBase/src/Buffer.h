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
		void setvec3(const GLfloat* verts, int cnt);
		void setvec2(const GLfloat* verts, int cnt);
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
