#include "Program.h"

namespace OpenGLBase
{
	GLuint Program::create()
	{
		this->data = glCreateProgram();
		return this->data;
	}

	void Program::attach(Shader& shader)
	{
		return glAttachShader(this->data, shader.data);
	}

	void Program::link()
	{
		return glLinkProgram(this->data);
	}

	void Program::debug()
	{
		int success;
		char infoLog[512];
		glGetProgramiv(this->data, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->data, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

	void Program::use()
	{
		return glUseProgram(this->data);
	}

	void Program::erase()
	{
		return glDeleteProgram(this->data);
	}
}
