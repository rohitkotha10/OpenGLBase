#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace OpenGLBase
{
	class VertexArray
	{
		GLuint data;
	public:
		GLuint getData() { return data; };
		void create();
		void bind();
		void unbind();
		void setAttrib(GLuint destination, GLint size, GLenum type);//assuming tightly packed and no offset
		void enable(GLuint destination);
		void erase();
	};
}

