#pragma once

#include "../GLCommon.h"
#include "Window.h"
#include "Layer.h"
#include "Overlay.h"


namespace GL {

	struct ApplicationConfig
	{
		const char* Name = "OpenGL-Sandbox";
		const char* GLSLVersion = "#version 460";
		uint16_t Width = 1280;
		uint16_t Height = 720;
	};

	class Application {
	public:
		Application() = default;
		~Application();

		void Init(const ApplicationConfig& config);
		void Run();

		void PushLayer(Layer* layer);
		void OnEvent(Event& event);

		void* GetWindow() { return m_Window.Get(); };

	public:
		static Application* Get() { return s_AppInstance; };

	private:
		bool OnWindowClose(WindowCloseEvent& event);

	private:
		Window m_Window;
		std::vector<Layer*> m_LayerStack;
		Overlay* m_Overlay;

		bool m_Running = true;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_AppInstance;

	};


};


