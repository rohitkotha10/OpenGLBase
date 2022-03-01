#include "OpenGLBase.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace OpenGLBase;

int curImguiWindow = 0;
bool hideCursor = true;
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
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (hideCursor)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			hideCursor = !hideCursor;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			hideCursor = !hideCursor;
		}

		firstMouse = true;
	}
};

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (hideCursor == false)
		return;

	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (hideCursor == false)
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
	Program programBackpack;
	Program programLight;

	VertexArray vaoLight;

	VertexBuffer vertCoord;
	IndexBuffer vertIndices;

	Scene object;

	glm::mat4 proj_matrix = glm::mat4(1.0f);
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 model_matrix = glm::mat4(1.0f);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	float objectRotate[3] = { 0.0f, 0.0f, 0.0f };
	float objectScale = 1.0f;

	glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 5.0f);
	glm::vec3 lightScale = glm::vec3(0.2f);
	float lightAmbient = 0.2f;
	float lightDiffuse = 0.5f;
	float lightSpecular = 1.0f;

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

public:
	void init()
	{
		info.width = 1920;
		info.height = 1080;
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

		programBackpack.create();
		programBackpack.attach(vs);
		programBackpack.attach(fs);
		programBackpack.link();
		programBackpack.debug();

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

		object.source("res/media/backpack/backpack.obj", true);
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

		programBackpack.use();
		programBackpack.setMat4("proj_matrix", proj_matrix);
		programBackpack.setMat4("view_matrix", view_matrix);
		programBackpack.setVec3("viewPos", cameraPos);
		programBackpack.setVec3("light.position", lightPos);
		programBackpack.setFloat("light.ambient", lightAmbient);
		programBackpack.setFloat("light.diffuse", lightDiffuse);
		programBackpack.setFloat("light.specular", lightSpecular);

		model_matrix =
			glm::translate(glm::mat4(1.0f), objectPos);
		model_matrix =
			glm::scale(model_matrix, glm::vec3(objectScale));
		model_matrix =
			glm::rotate(model_matrix, glm::radians(objectRotate[0]), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(model_matrix, glm::radians(objectRotate[1]), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(model_matrix, glm::radians(objectRotate[2]), glm::vec3(0.0f, 0.0f, 1.0f));
		programBackpack.setMat4("model_matrix", model_matrix);

		object.drawScene(programBackpack);



		programLight.use();
		model_matrix =
			glm::translate(glm::mat4(1.0f), lightPos);
		model_matrix =
			glm::scale(model_matrix, lightScale);

		programLight.setMat4("proj_matrix", proj_matrix);
		programLight.setMat4("view_matrix", view_matrix);
		programLight.setMat4("model_matrix", model_matrix);

		vaoLight.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		if (hideCursor)
			this->onKeyUpdate();
	}

	void imguiRender(double currentTime)
	{
		//ImGui::ShowDemoWindow();
		//return;
		

		if (hideCursor)
			return;


		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::Begin("OpenGLBase", 0, window_flags);

		switch (curImguiWindow)
		{
		case 0:
		{
			if (ImGui::Button("Light Settings"))
				curImguiWindow = 1;
			if (ImGui::Button("Object Settings"))
				curImguiWindow = 2;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Press Escape to Exit");
			break;
		}

		case 1:
		{
			if (ImGui::Button("<-"))
				curImguiWindow = 0;

			ImGui::SliderFloat3("Light Translation", &lightPos.x, -30.0f, 30.0f);
			ImGui::SliderFloat("Ambient Strength", &lightAmbient, 0.1f, 10.0f);
			ImGui::SliderFloat("Diffuse Strength", &lightDiffuse, 0.1f, 10.0f);
			ImGui::SliderFloat("Specular Strength", &lightSpecular, 0.1f, 10.0f);
			break;
		}

		case 2:
		{
			if (ImGui::Button("<-"))
				curImguiWindow = 0;

			ImGui::SliderFloat3("Object Translation", &objectPos.x, -5.0f, 5.0f);
			ImGui::SliderFloat3("Object Rotation", &objectRotate[0], -360.0f, 360.0f);
			ImGui::SliderFloat("Object Scale", &objectScale, 0.1f, 3.0f);
			break;
		}

		default:
			break;
		}
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
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp;
	}

	void shutdown()
	{
		programBackpack.erase();
		programLight.erase();
		vaoLight.erase();
		vertCoord.erase();
		vertIndices.erase();
	}

};

START_APP(my_app);
