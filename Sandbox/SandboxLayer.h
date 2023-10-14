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
	virtual void OnRemove();
	virtual void OnOverlayRender();

	virtual void OnUpdate(float deltaTime);
	virtual void OnEvent(Event& event);

private:
	unsigned int m_CubeVA, m_CubeVB, m_CubeIB;

	ShaderSystem* m_ShaderSystem = nullptr;
	CameraSystem* m_CameraSystem = nullptr;
};

