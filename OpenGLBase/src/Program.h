#pragma once
#include "Shader.h"

#include <glm/glm.hpp>

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
		void setMat4(std::string name, const glm::mat4& data);
		void setVec3(std::string name, float x, float y, float z);
		void setVec3(std::string name, const glm::vec3& data);
		void setTexture(std::string name, int destination);
	};
}

