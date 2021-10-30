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
	VertexArray vao;

	VertexBuffer vertCoord;
	VertexBuffer texCoord;

	IndexBuffer vertIndices;

	Texture tex1;
	Texture tex2;

	Uniform myTex1;
	Uniform myTex2;
	Uniform model;
	Uniform view;
	Uniform projection;

	glm::mat4 proj_matrix = glm::mat4(1.0f);
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;
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

		vs.create(VERTEX);
		vs.source("res/shaders/vs.shader");
		vs.compile();
		vs.debug();

		fs.create(FRAGMENT);
		fs.source("res/shaders/fs.shader");
		fs.compile();
		fs.debug();

		program.create();
		program.attach(vs);
		program.attach(fs);
		program.link();
		program.debug();

		vs.erase();
		fs.erase();
	}

	void startup()
	{

		glfwSetKeyCallback(window, keyboard_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		vao.create();
		vao.bind();

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

		static const float tex_positions[] =
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

		//buffers
		vertCoord.create();
		vertCoord.bind();
		vertCoord.setvec3f(vertex_positions, 24, 0);

		texCoord.create();
		texCoord.bind();
		texCoord.setvec2f(tex_positions, 24, 1);

		vertIndices.create();
		vertIndices.bind();
		vertIndices.setIndices(indices, 36);


		//textures
		flipTexture(true);
		tex1.create();
		tex1.bind();
		tex1.setTexture("res/media/wall.jpg");

		tex2.create();
		tex2.bind();
		tex2.setTexture("res/media/smile.jpg");
	}

	void render(double currentTime)
	{
		glClearBufferfi(GL_DEPTH_STENCIL, 0, 1, 0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		program.use();

		myTex1.create(program, "myTex1");
		myTex1.set1i(0);
		tex1.activate(TEX0);

		myTex2.create(program, "myTex2");
		myTex2.set1i(1);
		tex2.activate(TEX1);

		model.create(program, "model_matrix");
		view.create(program, "view_matrix");
		projection.create(program, "proj_matrix");

		float time = (float)currentTime;

		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float aspect = (float)info.width / (float)info.height;

		proj_matrix =
			glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);

		view_matrix =
			glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		model_matrix =
			glm::translate(glm::mat4(1.0f), translate) *
			glm::rotate(glm::mat4(1.0f), time * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), time * 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		projection.setMat4fv(1, false, glm::value_ptr(proj_matrix));
		view.setMat4fv(1, false, glm::value_ptr(view_matrix));
		model.setMat4fv(1, false, glm::value_ptr(model_matrix));

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
		texCoord.erase();
		vertIndices.erase();
		tex1.erase();
		tex2.erase();
	}

};

START_APP(my_app);
