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
	void Program::setInt(std::string name, int value)
	{
		glUniform1i(glGetUniformLocation(this->data, name.c_str()), value);
	}
	void Program::setFloat(std::string name, float value)
	{
		glUniform1f(glGetUniformLocation(this->data, name.c_str()), value);
	}
	void Program::setBool(std::string name, bool value)
	{
		glUniform1i(glGetUniformLocation(this->data, name.c_str()), value);
	}
	void Program::setMat4(std::string name, int count, bool transpose, float* data)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->data, name.c_str()), count, transpose, data);
	}
	void Program::setVec4(std::string name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(this->data, name.c_str()), x, y, z, w);
	}

	void Program::setTexture(std::string name, int destination)
	{
		glUniform1i(glGetUniformLocation(this->data, name.c_str()), destination);
	}

}
