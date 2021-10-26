#include "OpenGLBase.h";

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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

class my_app : public OpenGLBase::OpenGLApp
{
	GLuint rendering_program;
	GLuint vao;

	GLuint vertBuffer;
	GLuint indBuffer;
	GLuint texBuffer;

	GLuint texture1;
	GLuint texture2;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


public:
	void init()
	{
		info.width = 1280;
		info.height = 720;
		info.title = "Camera";
	}

	void startup()
	{
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

 		//glfwSetCursorPosCallback(window, mouse_callback);
		//glfwSetScrollCallback(window, scroll_callback);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		static const GLfloat vertex_positions[] =
		{
			//z
			-0.25f, -0.25f, -0.25f,//0
			0.25f, -0.25f, -0.25f,//1
			0.25f, 0.25f, -0.25f,//2
			-0.25f, 0.25f, -0.25f,//3

			0.25f, -0.25f, -0.25f,//1
			0.25f, -0.25f, 0.25f,//4
			0.25f, 0.25f, 0.25f,//5
			0.25f, 0.25f, -0.25f,//2

			-0.25f, -0.25f, 0.25f,//7
			-0.25f, -0.25f, -0.25f,//0
			-0.25f, 0.25f, -0.25f,//3
			-0.25f, 0.25f, 0.25f,//6

			0.25f, -0.25f, 0.25f,//4
			-0.25f, -0.25f, 0.25f,//7
			-0.25f, 0.25f, 0.25f,//6
			0.25f, 0.25f, 0.25f,//5

			-0.25f, 0.25f, -0.25f,//3
			0.25f, 0.25f, -0.25f,//2
			0.25f, 0.25f, 0.25f,//5
			-0.25f, 0.25f, 0.25f,//6

			-0.25f, -0.25f, -0.25f,//0
			0.25f, -0.25f, -0.25f,//1
			0.25f, -0.25f, 0.25f,//4
			-0.25f, -0.25f, 0.25f,//7
		};

		static const int indices[] =
		{
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
		};

		static const float texCoords[] =
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
		};

		glGenBuffers(1, &vertBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

		glGenBuffers(1, &indBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &texBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
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
		static unsigned char* data = stbi_load("res/media/wall.jpg", &width, &height, &nrChannels, 0);

		if (!data)
			std::cout << "Fail Texture Load" << std::endl;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		data = stbi_load("res/media/smile.jpg", &width, &height, &nrChannels, 0);

		if (!data)
			std::cout << "Fail Texture Load" << std::endl;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	void render(double currentTime)
	{

		GLfloat color[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glUseProgram(rendering_program);

		glUniform1i(glGetUniformLocation(rendering_program, "myTex1"), 0);
		glUniform1i(glGetUniformLocation(rendering_program, "myTex2"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		int proj_location = glGetUniformLocation(rendering_program, "proj_matrix");
		int view_location = glGetUniformLocation(rendering_program, "view_matrix");
		int model_location = glGetUniformLocation(rendering_program, "model_matrix");

		float time = (float)currentTime;

		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

		float aspect = (float)info.width / (float)info.height;
		glm::mat4 proj_matrix = glm::mat4(1.0f);
		glm::mat4 view_matrix = glm::mat4(1.0f);
		glm::mat4 model_matrix = glm::mat4(1.0f);

		proj_matrix =
			glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);

		view_matrix =
			glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj_matrix));
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

		static glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{

			ImGui::Begin("Translate");
			ImGui::SliderFloat3("float", &translate.x, -3.0f, 3.0f);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		model_matrix =
			glm::translate(glm::mat4(1.0f), translate) *
			glm::rotate(glm::mat4(1.0f), time * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), time * 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);




	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(rendering_program);
		glDeleteBuffers(1, &vertBuffer);
		glDeleteBuffers(1, &indBuffer);
		glDeleteBuffers(1, &texBuffer);
		glDeleteTextures(1, &texture1);
		glDeleteTextures(1, &texture2);
	}

};

START_APP(my_app);
