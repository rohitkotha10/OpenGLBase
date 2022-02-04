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
	void Program::setMat4(std::string name, const glm::mat4& data)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->data, name.c_str()), 1, GL_FALSE, &data[0][0]);
	}
	void Program::setVec3(std::string name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(this->data, name.c_str()), x, y, z);
	}

	void Program::setVec3(std::string name, const glm::vec3& data)
	{
		glUniform3fv(glGetUniformLocation(this->data, name.c_str()), 1, &data[0]);
	}

	void Program::setTexture(std::string name, int destination)
	{
		glUniform1i(glGetUniformLocation(this->data, name.c_str()), destination);
	}

}
