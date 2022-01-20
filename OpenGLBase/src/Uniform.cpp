#include "Uniform.h"

namespace OpenGLBase
{
	void Uniform::create(Program program, std::string name)
	{
		this->program = program;
		this->name = name;
		this->location = glGetUniformLocation((this->program).getData(), (this->name).c_str());
	}
	void Uniform::set1i(int value)
	{
		glUniform1i(this->location, value);
	}
	void Uniform::setMat4fv(int count, bool transpose, float* data)
	{
		glUniformMatrix4fv(this->location, count, transpose, data);
	}
	void Uniform::setVec4(float x, float y, float z, float w)
	{
			glUniform4f(this->location, x, y, z, w);
	}
}
