#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include "Program.h"

namespace OpenGLBase
{
	class Uniform
	{
		Program program;
		int location;
		std::string name;
	public:
		void create(Program program, std::string name);
		void set1i(int value);
		void setMat4fv(int count, bool transpose, float* data);
		void setVec4(float x, float y, float z, float w);
		int getLocation() { return location; };
	};
}
