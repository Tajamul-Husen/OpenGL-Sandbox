#include "Window.h"


namespace GL {

	Window::~Window()
	{
		Destroy();
	};


	void Window::Create(const WindowConfig& config)
	{
		m_WindowData.Title = config.Title;
		m_WindowData.Width = config.Width;
		m_WindowData.Height = config.Height;

		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // require for debugging enable in opengl

		m_Window = glfwCreateWindow((int)m_WindowData.Width, (int)m_WindowData.Height, m_WindowData.Title, NULL, NULL);

		GL_ASSERT(m_Window != NULL, "Failed to create GLFW window.");

		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		GL_ASSERT(status, "Failed to intialize GLAD.");

		GL_LOG_INFO("OpenGL Info: ");
		GL_LOG_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		GL_LOG_INFO("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		GL_LOG_INFO("Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		glfwSwapInterval(1); // For VSYNC

		glfwSetWindowUserPointer(m_Window, &m_WindowData);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowResizeEvent event(width, height);
				data->EventCallback(event);
			}
		);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent event;
				data->EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data->EventCallback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data->EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data->EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseMovedEvent event((float)xPos, (float)yPos);
				data->EventCallback(event);
			});
	};


	void Window::Update() {
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	};


	void Window::Destroy() {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	};

};
