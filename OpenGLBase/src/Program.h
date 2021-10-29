#pragma once
#include "Shader.h"

namespace OpenGLBase
{
	class Program
	{
		GLuint data;
	public:
		GLuint getData() { return data; };
		void create();
		void attach(Shader& shader );
		void link();
		void debug();
		void use();
		void erase();
	};
}

