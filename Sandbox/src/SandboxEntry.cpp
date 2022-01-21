#include "OpenGLBase.h"

using namespace OpenGLBase;

bool useCursor = true;
bool firstMouse = true;
float fov = 45.0f;

void  keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		if (useCursor)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			useCursor = !useCursor;
		}

		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			useCursor = !useCursor;
		}
		firstMouse = true;
	}
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

class my_app : public OpenGLApp
{
	Program program;
	Program programLight;

	VertexArray vao;
	VertexArray vaoLight;

	VertexBuffer vertCoord;
	IndexBuffer vertIndices;

	glm::mat4 proj_matrix = glm::mat4(1.0f);
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 translate2 = glm::vec3(2.0f, 0.0f, 0.0f);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;//horizontal
	float pitch = 0.0f;//vertical
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;		

public:
	void init()
	{
		info.width = 1280;
		info.height = 720;
		info.title = "OpenGLBase";
		info.color = new float[4]{ 0.25f, 0.35f, 0.35f, 1.0f };
		info.fullscreen = 0;
	}

	void shaderCompile()
	{
		Shader vs;
		Shader fs;
		Shader vsLight;
		Shader fsLight;

		vs.create(VERTEX);
		vs.source("res/shaders/vs.shader");
		vs.compile();
		vs.debug();

		fs.create(FRAGMENT);
		fs.source("res/shaders/fs.shader");
		fs.compile();
		fs.debug();

		vsLight.create(VERTEX);
		vsLight.source("res/shaders/vsLight.shader");
		vsLight.compile();
		vsLight.debug();

		fsLight.create(FRAGMENT);
		fsLight.source("res/shaders/fsLight.shader");
		fsLight.compile();
		fsLight.debug();

		program.create();
		program.attach(vs);
		program.attach(fs);
		program.link();
		program.debug();

		programLight.create();
		programLight.attach(vsLight);
		programLight.attach(fsLight);
		programLight.link();
		programLight.debug();

		vs.erase();
		fs.erase();
		vsLight.erase();
		fsLight.erase();
	}	

	void startup()
	{

		glfwSetKeyCallback(window, keyboard_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		static const GLuint indices[] =
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


		vao.create();
		vaoLight.create();
		vertCoord.create();
		vertIndices.create();

		vao.bind();

		vertCoord.bind();
		vertCoord.setvec3(vertex_positions, 24);
		vao.setAttrib(0, 3, GL_FLOAT);
		vao.enable(0);
		vertIndices.bind();
		vertIndices.setIndices(indices, 36);

		vaoLight.bind();

		vertCoord.bind();
		vertCoord.setvec3(vertex_positions, 24);
		vao.setAttrib(0, 3, GL_FLOAT);
		vao.enable(0);
		vertIndices.bind();
		vertIndices.setIndices(indices, 36);
	}

	void render(double currentTime)
	{
		glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		float time = (float)currentTime;

		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float aspect = (float)info.width / (float)info.height;

		program.use();

		proj_matrix =
			glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);

		view_matrix =
			glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		model_matrix =
			glm::translate(glm::mat4(1.0f), translate) *
			glm::rotate(glm::mat4(1.0f), 2.0f * time, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), 2.0f * time, glm::vec3(1.0f, 0.0f, 0.0f));

		program.setVec4("objectColor", 1.0f, 0.5f, 0.31f, 1.0f);
		program.setVec4("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);

		program.setMat4("proj_matrix", 1, false, glm::value_ptr(proj_matrix));
		program.setMat4("view_matrix", 1, false, glm::value_ptr(view_matrix));
		program.setMat4("model_matrix", 1, false, glm::value_ptr(model_matrix));

		vao.bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		programLight.use();
		model_matrix =
			glm::translate(glm::mat4(1.0f), translate2);

		programLight.setMat4("proj_matrix", 1, false, glm::value_ptr(proj_matrix));
		programLight.setMat4("view_matrix", 1, false, glm::value_ptr(view_matrix));
		programLight.setMat4("model_matrix", 1, false, glm::value_ptr(model_matrix));

		vaoLight.bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		this->onKeyUpdate();
		if (useCursor)
			this->onCursor();
	}

	void imguiRender(double currentTime)
	{

		ImGui::Begin("OpenGLBase");
		ImGui::SliderFloat3("Cube Translation", &translate.x, -3.0f, 3.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Text("Press Left Ctrl to Enable/Disable Cursor");
		ImGui::Text("Press Escape to Exit");
		ImGui::End();
	}

	void onKeyUpdate()
	{
		const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	void onCursor()
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

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

		if (pitch > 115.0f)
			pitch = 115.0f;
		if (pitch < -115.0f)
			pitch = -115.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}

	void shutdown()
	{
		program.erase();
		vao.erase();
		vertCoord.erase();
		vertIndices.erase();
	}

};

START_APP(my_app);
