#include "OpenGLBase.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace OpenGLBase;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Mesh(std::vector<Vertex> vert, std::vector<unsigned int> ind)
		:vertices(vert), indices(ind) {};
};

class Model
{
public:
	Model(std::string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ASSIMP Import Error" << import.GetErrorString() << std::endl;
			return;
		}
		processNode(scene->mRootNode, scene);
	};
	
	std::vector<Mesh> meshes;

	void processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return Mesh(vertices, indices);
	}
};

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
	Program backProg;

	VertexArray vao;
	VertexArray vaoLight;
	VertexArray backvao;

	VertexBuffer vertCoord;
	IndexBuffer vertIndices;
	VertexBuffer back;
	IndexBuffer backInd;

	Texture texDiff;
	Texture texSpec;

	glm::mat4 proj_matrix = glm::mat4(1.0f);
	glm::mat4 view_matrix = glm::mat4(1.0f);
	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
	glm::vec3 backPos = glm::vec3(0.0f, 0.0f, 0.0f);

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
		info.fullscreen = 0;
	}

	void shaderCompile()
	{
		Shader vs;
		Shader fs;
		Shader vsLight;
		Shader fsLight;
		Shader vsback;
		Shader fsback;

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

		vsback.create(VERTEX);
		vsback.source("res/shaders/vsback.shader");
		vsback.compile();
		vsback.debug();

		fsback.create(FRAGMENT);
		fsback.source("res/shaders/fsback.shader");
		fsback.compile();
		fsback.debug();

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

		backProg.create();
		backProg.attach(vsback);
		backProg.attach(fsback);
		backProg.link();
		backProg.debug();

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

		//Model check("res/media/backpack.obj");
		//int ver = 0;
		//int indi = 0;
		//static std::vector<float> backarr;
		//static std::vector<unsigned int> backarrInd;
		//for (int i = 0; i < check.meshes.size(); i++)
		//{
		//	ver += check.meshes[i].vertices.size();
		//	indi += check.meshes[i].indices.size();

		//	for (int j = 0; j < check.meshes[i].vertices.size(); j++)
		//	{
		//		backarr.push_back(check.meshes[i].vertices[j].position.x);
		//		backarr.push_back(check.meshes[i].vertices[j].position.y);
		//		backarr.push_back(check.meshes[i].vertices[j].position.z);
		//	}
		//	for (int j = 0; j < check.meshes[i].indices.size(); j++)
		//	{
		//		backarrInd.push_back(check.meshes[i].indices[i]);
		//	}
		//}
		//size = ver;

		vao.create();
		vaoLight.create();
		vertCoord.create();
		vertIndices.create();
		texDiff.create();
		texSpec.create();

		vao.bind();

		vertCoord.bind();
		vertCoord.setvec3(vertices, sizeof(vertices));

		vao.setAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		vao.enable(0);
		vao.setAttrib(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		vao.enable(1);
		vao.setAttrib(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		vao.enable(2);

		texDiff.bind();
		texDiff.setTexture("res/media/wood.jpg", true);
		texSpec.bind();
		texSpec.setTexture("res/media/woodspec.jpg", true);

		texDiff.active(GL_TEXTURE0);
		texDiff.bind();
		texSpec.active(GL_TEXTURE1);
		texSpec.bind();

		vaoLight.bind();

		vertCoord.bind();
		vao.setAttrib(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		vao.enable(0);
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

		program.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		program.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		program.setVec3("light.position", lightPos);
		program.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		program.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		program.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		program.setTexture("material.diffuse", 0);
		program.setTexture("material.specular", 1);
		program.setFloat("material.shininess", 32.0f);

		program.setVec3("viewPos", cameraPos);

		program.setMat4("proj_matrix", proj_matrix);
		program.setMat4("view_matrix", view_matrix);
		program.setMat4("model_matrix", model_matrix);

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

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

		//backProg.use();
		//model_matrix =
		//	glm::translate(glm::mat4(1.0f), backPos);

		//backProg.setMat4("proj_matrix", 1, false, glm::value_ptr(proj_matrix));
		//backProg.setMat4("view_matrix", 1, false, glm::value_ptr(view_matrix));
		//backProg.setMat4("model_matrix", 1, false, glm::value_ptr(model_matrix));

		//backvao.bind();
		//glDrawArrays(GL_TRIANGLES, 0, size);

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
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
