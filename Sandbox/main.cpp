#include "SandboxLayer.h"
#include "Sandbox2DLayer.h"


using namespace GL;

int main()
{
	Application sandbox;
	ApplicationConfig config;

	sandbox.Init(config);

	sandbox.PushLayer(new SandboxLayer("Render Layer"));
	//sandbox.PushLayer(new Sandbox2DLayer("2D Render Layer"));

	sandbox.Run();

	return 0;
}


