#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace OpenGLBase
{
	class VertexArray
	{
		GLuint id;
	public:
		GLuint getId() { return id; };
		void create();
		void bind();
		void unbind();
		void setAttrib(GLuint destination, GLint size, GLenum type, GLsizei stride, const void* pointer);
		void enable(GLuint destination);
		void erase();
	};
}

