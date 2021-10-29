#pragma once
#include "Shader.h"

namespace OpenGLBase
{
	class Program
	{
	public:
		GLuint data;
		GLuint create();
		void attach(Shader& shader );
		void link();
		void debug();
		void use();
		void erase();
	};
}

