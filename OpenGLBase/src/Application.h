#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>

namespace OpenGLBase
{
	void processInput(GLFWwindow* window);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	class OpenGLApp
	{
	public:
		OpenGLApp() {};
		virtual ~OpenGLApp() {};
		virtual void init();
		virtual void run();
		virtual void startup();
		virtual void render(double currentTime);
		virtual void shutdown();
		struct APPINFO
		{
			std::string title = "Welcome";
			int width = 800;
			int height = 600;
			int majorVersion = 4;
			int minorVersion = 3;
		};

	protected:
		APPINFO info;
		GLFWwindow* window = NULL;
	};
};

std::string parse(std::string shadertype);


#define START_APP(my_app)        \
int main()                       \
{                                \
	my_app* app = new my_app;    \
	app->run();                  \
	delete app;                  \
	return 0;                    \
}                                \


