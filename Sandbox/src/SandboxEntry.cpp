#include "OpenGLBase.h";
#include "stb_image.h";

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
	GLuint indibuffer;
	GLuint texbuffer;
	GLuint texture1;
	GLuint texture2;

public:

	void init()
	{
		info.width = 1000;
		info.height = 1000;
		info.title = "Textures";
	}

	void startup()
	{
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		static const float vertex_positions[] =
		{
			//z
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		static const int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		static const float texCoords[] =
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f

		};

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

		glGenBuffers(1, &indibuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indibuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &texbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		stbi_set_flip_vertically_on_load(true);

		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		static unsigned char* data = stbi_load("../../../Sandbox/res/media/wall.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Fail Texture Load" << std::endl;
		}

		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		data = stbi_load("../../../Sandbox/res/media/smile.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Fail Texture Load" << std::endl;
		}

		
		stbi_image_free(data);

	}

	void render(double currentTime)
	{

		GLfloat color[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));


		glUseProgram(rendering_program);

		glUniform1i(glGetUniformLocation(rendering_program, "myTex1"), 0);
		glUniform1i(glGetUniformLocation(rendering_program, "myTex2"), 1);

		unsigned int transformLoc = glGetUniformLocation(rendering_program, "transMat");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(rendering_program);
		glDeleteBuffers(1, &buffer);
		glDeleteBuffers(1, &indibuffer);
		glDeleteBuffers(1, &texbuffer); 
		glDeleteTextures(1, &texture1);
		glDeleteTextures(1, &texture2);
	}

};

START_APP(my_app);
