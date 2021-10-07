#include "../OpenGLApp.h"
#include "vmath.h"

GLuint compile_shaders(void)
{
	GLuint vs;
	GLuint fs;
	GLuint program;

	std::string vs_string = parse("res/shaders/vs.shader");
	static const GLchar* vs_source[] = { vs_string.c_str() };

	std::string fs_string = parse("res/shaders/fs.shader");
	static const GLchar* fs_source[] = { fs_string.c_str() };

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);
	int success;
	char infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL); //copy code
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

class my_app : public OpenGLBase::OpenGLApp
{
	GLuint rendering_program;
	GLuint vao;
	GLuint buffer;
	GLint mv_location;
	GLint proj_location;

public:

	void init()
	{
		info.width = 800;
		info.height = 600;
		info.title = "Hello";
	}

	void startup()
	{
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &buffer);

		static const GLfloat vertex_positions[] =
		{
			//z
			0.25f, 0.25f, -0.25f,
			0.25f, -0.25f, -0.25f,
			-0.25f, 0.25f, -0.25f,

			0.25f, -0.25f, -0.25f,
			-0.25f, 0.25f, -0.25f,
			-0.25f, -0.25f, -0.25f,

			0.25f, 0.25f, 0.25f,
			0.25f, -0.25f, 0.25f,
			-0.25f, 0.25f, 0.25f,

			0.25f, -0.25f, 0.25f,
			-0.25f, 0.25f, 0.25f,
			-0.25f, -0.25f, 0.25f,
			//z

			//x
			0.25f, 0.25f, 0.25f,
			0.25f, 0.25f, -0.25f,
			0.25f, -0.25f, 0.25f,

			0.25f, 0.25f, -0.25f,
			0.25f, -0.25f, 0.25f,
			0.25f, -0.25f, -0.25f,

			-0.25f, 0.25f, 0.25f,
			-0.25f, 0.25f, -0.25f,
			-0.25f, -0.25f, 0.25f,

			-0.25f, 0.25f, -0.25f,
			-0.25f, -0.25f, 0.25f,
			-0.25f, -0.25f, -0.25f,
			//x

			//y
			0.25f, 0.25f, 0.25f,
			0.25f, 0.25f, -0.25f,
			-0.25f, 0.25f, 0.25f,

			0.25f, 0.25f, -0.25f,
			-0.25f, 0.25f, 0.25f,
			-0.25f, 0.25f, -0.25f,

			0.25f, -0.25f, 0.25f,
			0.25f, -0.25f, -0.25f,
			-0.25f, -0.25f, 0.25f,

			0.25f, -0.25f, -0.25f,
			-0.25f, -0.25f, 0.25f,
			-0.25f, -0.25f, -0.25f
			//y		
		};


		mv_location = glGetUniformLocation(rendering_program, "mv_matrix");
		proj_location = glGetUniformLocation(rendering_program, "proj_matrix");

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}

	void render(double currentTime)
	{

		GLfloat color[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glUseProgram(rendering_program);

		vmath::mat4 mv_matrix;
		float f;

		float aspect = (float)info.width / (float)info.height;
		vmath::mat4 proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, proj_matrix);

		f = (float)currentTime * 0.1f;
		mv_matrix =
			vmath::translate(0.0f, 0.0f, -4.0f) *
			vmath::translate
			(sinf(2.1f * f) * 0.5f,
				cosf(1.7f * f) * 0.5f,
				sinf(1.3f * f) * cosf(1.5f * f) * 2.0f
			) *
			vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
			vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);

		glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv_matrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &buffer);
		glDeleteProgram(rendering_program);
	}

};

START_APP(my_app);
