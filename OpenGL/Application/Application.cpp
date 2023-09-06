#include "Application.h"


namespace GL {

	Application* Application::s_AppInstance = nullptr;

	Application::~Application()
	{
		for (auto& layer : m_LayerStack)
			delete layer;

		m_LayerStack.clear();
	};


	void Application::Init(const ApplicationConfig& config)
	{
		if (!s_AppInstance) s_AppInstance = this;

		m_Window.Create(WindowConfig(config.Name, config.Width, config.Height));
		m_Window.SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	};


	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.push_back(layer);
		layer->OnAdd();
		GL_LOG_INFO("New Layer Added: {0} ", layer->GetName());
	};


	void Application::Run()
	{
		while (m_Running)
		{
			float currentFrame = m_Window.GetTime();
			float m_DeltaFrameTime = currentFrame - m_LastFrameTime;
			m_LastFrameTime = currentFrame;

			// Layers Rendering
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(m_DeltaFrameTime);

			// window 
			m_Window.Update();
		}
	};


	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(event);
			if (event.IsHandled())
				break;
		}
	};


	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	};

};
