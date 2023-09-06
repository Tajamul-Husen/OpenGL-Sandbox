#pragma once

#include <OpenGL.h>
#include <string>


using namespace GL;

class SandboxLayer : public Layer
{
	public:
		SandboxLayer(const std::string& name);
		~SandboxLayer();

		virtual void OnAdd();
		virtual void OnUpdate(float deltaTime);
		virtual void OnEvent(Event& event);
		virtual void OnRemove();
};

