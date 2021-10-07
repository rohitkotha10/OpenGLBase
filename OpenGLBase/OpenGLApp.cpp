#include "OpenGLApp.h"

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

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		startup();

		do
		{
			processInput(window);

			render(glfwGetTime());

			glfwSwapBuffers(window);
			glfwPollEvents();
		} while (!glfwWindowShouldClose(window));

		shutdown();

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
	std::string rootpath = "../../../OpenGLBase/";
	rootpath += shadertype;
	shadertype = rootpath;

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
