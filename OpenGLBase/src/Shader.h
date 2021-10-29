#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <iostream>

namespace OpenGLBase
{
	enum ShaderType
	{
		VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER
	};

	class Shader
	{
	public:
		const GLchar* add;
		GLuint data;
		GLuint create(ShaderType type);
		void source(std::string shaderPath);
		void compile();
		void debug();
		void erase();
	};
}