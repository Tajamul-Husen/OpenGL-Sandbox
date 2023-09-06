#include "SandboxLayer.h"


SandboxLayer::SandboxLayer(const std::string& name) : Layer(name) {};
SandboxLayer::~SandboxLayer() {};

void SandboxLayer::OnAdd()
{
	EnableDebugging();
};


void SandboxLayer::OnUpdate(float deltaTime)
{
	// camera update

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};


void SandboxLayer::OnEvent(Event& event)
{
	// camera event

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent& ev) { glViewport(0, 0, ev.GetWidth(), ev.GetHeight()); return true; });

};


void SandboxLayer::OnRemove() {};


