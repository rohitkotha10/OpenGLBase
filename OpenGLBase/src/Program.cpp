#include "Program.h"

namespace OpenGLBase
{
	void Program::create()
	{
		this->data = glCreateProgram();
	}

	void Program::attach(Shader& shader)
	{
		glAttachShader(this->data, shader.getData());
	}

	void Program::link()
	{
		glLinkProgram(this->data);
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
		glUseProgram(this->data);
	}

	void Program::erase()
	{
		glDeleteProgram(this->data);
	}
}
