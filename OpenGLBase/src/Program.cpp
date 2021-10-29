#include "Program.h"

GLuint OpenGLBase::Program::create()
{
	this->data = glCreateProgram();
	return this->data;
}

void OpenGLBase::Program::attach(Shader& shader)
{
	return glAttachShader(this->data, shader.data);
}

void OpenGLBase::Program::link()
{
	return glLinkProgram(this->data);
}

void OpenGLBase::Program::debug()
{
	int success;
	char infoLog[512];
	glGetProgramiv(this->data, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->data, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void OpenGLBase::Program::use()
{
	return glUseProgram(this->data);
}

void OpenGLBase::Program::erase()
{
	return glDeleteProgram(this->data);
}
