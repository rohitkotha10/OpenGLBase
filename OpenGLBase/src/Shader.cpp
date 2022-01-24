#include "Shader.h"

namespace OpenGLBase
{
	void Shader::create(ShaderType type)
	{
		this->data = glCreateShader(type);
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
		this->shaderSource = final.c_str();
		glShaderSource(this->data, 1, &this->shaderSource, NULL);
	}

	void Shader::compile()
	{
		glCompileShader(this->data);
	}

	void Shader::debug()
	{
		int success;
		char infoLog[512];
		glGetShaderiv(this->data, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(this->data, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	void Shader::erase()
	{
		glDeleteShader(this->data);
	}
}