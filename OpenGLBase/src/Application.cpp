#include "Application.h"


namespace OpenGLBase
{

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	};

	void OpenGLApp::run()
	{
		init();

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(info.width,
			info.height,
			info.title.c_str(),
			info.fullscreen ? glfwGetPrimaryMonitor() : NULL,
			NULL);
		
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(window);
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

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		shaderCompile();
		startup();

		while (!glfwWindowShouldClose(window))
		{	

			glClearBufferfv(GL_COLOR, 0, info.color);

			render(glfwGetTime());

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			imguiRender(glfwGetTime());

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			
			glfwPollEvents();
			glfwSwapInterval(1);
			glfwSwapBuffers(window);
		}

		shutdown();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		
		glfwDestroyWindow(window);
		glfwTerminate();
	};
}