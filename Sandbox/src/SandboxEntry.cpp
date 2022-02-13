#include "OpenGLBase.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace OpenGLBase;

bool useCursor = true;
bool firstMouse = true;

float yaw = -90.0f;//horizontal
float pitch = -0.0f;//vertical
float fov = 45.0f;

float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (useCursor == false)
		return;

	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	//prevents flipping when out of bounds
};

class my_app : public OpenGLApp
{
	Program program;
	Program programLight;

	VertexArray vaoLight;

	VertexBuffer vertCoord;
	IndexBuffer vertIndices;

	Scene backpack;
	Scene tv;

	glm::mat4 proj_matrix = glm::mat4(1.0f);
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 tvPos = glm::vec3(2.0f, 0.0f, -2.0f);
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	int size;
public:
	void init()
	{
		info.width = 800;
		info.height = 600;
		info.title = "OpenGLBase";
		info.color = new float[4]{ 0.1f, 0.1f, 0.1f, 1.0f };
		info.fullscreen = false;
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
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		static const GLfloat vertices[] =
		{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
		};

		vaoLight.create();
		vertCoord.create();
		vertIndices.create();

		vaoLight.bind();

		vertCoord.bind();
		vertCoord.setvec3(vertices, sizeof(vertices));

		vaoLight.setAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		vaoLight.enable(0);

		tv.source("res/media/tv/retrotv0319.obj", false);
		backpack.source("res/media/backpack/backpack.obj", true);
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

		proj_matrix =
			glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		cameraFront = glm::normalize(direction);
		cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

		view_matrix =
			glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		program.use();
		model_matrix =
			glm::translate(glm::mat4(1.0f), objectPos);
		model_matrix =
			glm::scale(model_matrix, glm::vec3(0.1f));

		program.setMat4("proj_matrix", proj_matrix);
		program.setMat4("view_matrix", view_matrix);
		program.setMat4("model_matrix", model_matrix);
		
		backpack.drawScene(program);

		model_matrix =
			glm::translate(glm::mat4(1.0f), tvPos);
		program.setMat4("model_matrix", model_matrix);

		tv.drawScene(program);

		programLight.use();
		model_matrix =
			glm::translate(glm::mat4(1.0f), lightPos);
		model_matrix =
			glm::scale(model_matrix, glm::vec3(0.2f));

		programLight.setMat4("proj_matrix", proj_matrix);
		programLight.setMat4("view_matrix", view_matrix);
		programLight.setMat4("model_matrix", model_matrix);

		vaoLight.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		this->onKeyUpdate();
	}

	void imguiRender(double currentTime)
	{

		ImGui::Begin("OpenGLBase");
		ImGui::SliderFloat3("Cube Translation", &objectPos.x, -3.0f, 3.0f);

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
			cameraPos -= cameraSpeed * cameraRight;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraRight;
	}

	void shutdown()
	{
		program.erase();
		programLight.erase();
		vaoLight.erase();
		vertCoord.erase();
		vertIndices.erase();
	}

};

START_APP(my_app);
