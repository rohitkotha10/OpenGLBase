#include "Shader.h"

namespace OpenGLBase
{
	GLuint Shader::create(ShaderType type)
	{
		this->data = glCreateShader(type);
		return this->data;
	}
	void Shader::source(std::string shaderPath)
	{
		std::ifstream ifs(shaderPath);

		std::string final;

		int cnt = 0;
		while (!ifs.eof())
		{
			std::string temp;
			getline(ifs, temp);
			final += temp + '\n';
		}
		this->add = final.c_str();
		return glShaderSource(this->data, 1, &this->add, NULL);
	}

	void Shader::compile()
	{
		return glCompileShader(this->data);
	}

	void Shader::debug()
	{
		int success;
		char infoLog[512];
		glGetShaderiv(this->data, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(this->data, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	void Shader::erase()
	{
		return glDeleteShader(this->data);
	}
}