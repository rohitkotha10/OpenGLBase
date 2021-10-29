#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace OpenGLBase
{
	class VertexArray
	{
	public:
		GLuint data;
		void create();
		void bind();
		void unbind();
		void erase();
	};
}

