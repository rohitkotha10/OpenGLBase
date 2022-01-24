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
		void attach(Shader& shader);
		void link();
		void debug();
		void use();
		void erase();
		//uniforms
		void setInt(std::string name, int value);
		void setFloat(std::string name, float value);
		void setBool(std::string name, bool value);
		void setMat4(std::string name, int count, bool transpose, float* data);
		void setVec4(std::string name, float x, float y, float z, float w);
		void setTexture(std::string name, int destination);
	};
}

