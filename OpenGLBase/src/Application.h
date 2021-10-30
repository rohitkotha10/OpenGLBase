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

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace OpenGLBase
{

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	class OpenGLApp
	{
	public:
		OpenGLApp() {};

		virtual ~OpenGLApp() {};

		virtual void run();

		virtual void init() {};
		virtual void shaderCompile() {};
		virtual void startup() {};
		virtual void render(double currentTime) {};
		virtual void imguiRender(double currentTime) {};
		virtual void shutdown() {};

		struct WindowInfo
		{
			std::string title = "Welcome";
			int width = 800;
			int height = 600;
			float* color = new float[4]{ 0.15f, 0.15f, 0.15f, 0.15f };
			bool fullscreen = 0;
		};

		WindowInfo info;
		GLFWwindow* window = NULL;
	};
};

#define START_APP(my_app)        \
int main()                       \
{                                \
	my_app* app = new my_app;    \
	app->run();                  \
	delete app;                  \
	return 0;                    \
}                                \


