#include "Application.h"


namespace OpenGLBase
{
	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	};

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	};

	void OpenGLApp::run()
	{
		init();

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(info.width, info.height, info.title.c_str(), NULL, NULL);
		
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		glewInit();
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;
		}

		const char* glsl_version = "#version 450";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		startup();

		do
		{
			processInput(window);
			
			render(glfwGetTime());

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		} while (!glfwWindowShouldClose(window));

		shutdown();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		
		glfwDestroyWindow(window);
		glfwTerminate();
	};

	void OpenGLApp::init() {};
	void OpenGLApp::startup() {};
	void OpenGLApp::render(double currentTime) {};
	void OpenGLApp::shutdown() {};
}

std::string parse(std::string shadertype)
{
	std::ifstream ifs(shadertype);

	std::string final;

	int cnt = 0;
	while (!ifs.eof())
	{
		std::string temp;
		getline(ifs, temp);
		final += temp + '\n';
	}
	return final;
}
