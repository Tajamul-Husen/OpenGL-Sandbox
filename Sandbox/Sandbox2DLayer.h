#pragma once

#include <OpenGL.h>
#include <string>


using namespace GL;

class Sandbox2DLayer : public Layer
{
public:
	Sandbox2DLayer(const std::string& name);
	~Sandbox2DLayer();

	virtual void OnAdd();
	virtual void OnUpdate(float deltaTime);
	virtual void OnEvent(Event& event);
	virtual void OnRemove();

private:
	unsigned int m_TrigVA, m_TrigVB, m_TrigIB;

	static const unsigned int m_TrigAmount = 100;

	ShaderSystem* m_ShaderSystem = nullptr;
	CameraSystem* m_CameraSystem = nullptr;
};

