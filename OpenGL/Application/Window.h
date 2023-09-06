#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "../GLCommon.h"
#include "../Events/Event.h"
#include "../Debug/Log.h"
#include "../Debug/Assert.h"


namespace GL {

	struct WindowConfig {
		const char* Title;
		uint16_t Width;
		uint16_t Height;

		WindowConfig(const char* title = "OpenGL-Sandbox", uint16_t width = 1280, uint16_t height = 720) : Title(title), Width(width), Height(height) {};
	};

	class Window
	{
		typedef std::function<void(Event&)> EventCallbackFn;

	public:
		Window() = default;
		~Window();

		void Create(const WindowConfig& config);
		void Update();
		void Destroy();

		void* Get() const { return m_Window; };
		float GetTime() { return static_cast<float>(glfwGetTime()); }
		void SetEventCallback(const EventCallbackFn& cb) { m_WindowData.EventCallback = cb; };

	private:
		GLFWwindow* m_Window = nullptr;

		struct WindowData {
			const char* Title;
			uint16_t Width;
			uint16_t Height;
			EventCallbackFn EventCallback;
		};

		WindowData m_WindowData;
	};

}
