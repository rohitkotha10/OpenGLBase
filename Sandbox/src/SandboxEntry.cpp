#include "OpenGLBase.h"

GLuint compile_shaders(void)
{
	GLuint vs;
	GLuint fs;
	GLuint program;

	std::string vs_string = parse("../../../Sandbox/res/shaders/vs.shader");
	static const GLchar* vs_source[] = { vs_string.c_str() };

	std::string fs_string = parse("../../../Sandbox/res/shaders/fs.shader");
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
	glShaderSource(fs, 1, fs_source, NULL);
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
		info.title = "Textures";
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
			-0.25f, -0.25f, -0.25f,
			0.25f, -0.25f, -0.25f,
			0.25f, 0.25f, -0.25f,
			-0.25f, 0.25f, -0.25f,
			0.25f, -0.25f, 0.25f,
			0.25f, 0.25f, 0.25f,
			-0.25f, 0.25f, 0.25f,
			-0.25f, -0.25f, 0.25f,
		};

		static const int indices[] =
		{
			0, 1, 2,
			2, 3, 0,
			1, 2, 4,
			2, 4, 5,
			5, 7, 4,
			5, 7, 6,
			6, 3, 7,
			3, 7, 0,
			0, 1, 7,
			7, 1, 4,
			6, 2, 5,
			2, 6, 3
		};



		mv_location = glGetUniformLocation(rendering_program, "mv_matrix");
		proj_location = glGetUniformLocation(rendering_program, "proj_matrix");

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		static unsigned int indibuffer;
		glGenBuffers(1, &indibuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indibuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	void render(double currentTime)
	{

		GLfloat color[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glUseProgram(rendering_program);

		glm::mat4 mv_matrix = glm::mat4(1.0f);
		float f;

		float aspect = (float)info.width / (float)info.height;
		glm::mat4 proj_matrix = glm::perspective(glm::radians(50.0f), aspect, 0.1f, 1000.0f);
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj_matrix));

		f = (float)currentTime;
		mv_matrix =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
			glm::translate(glm::mat4(1.0f), glm::vec3(
				sinf(f) * 1.0f,
				cosf(f) * 1.0f,
				sinf(f) * cosf(f) * 2.0f)
			) *
			glm::rotate(glm::mat4(1.0f), (float)currentTime * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), (float)currentTime * 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(mv_matrix));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &buffer);
		glDeleteProgram(rendering_program);
	}

};

START_APP(my_app);
