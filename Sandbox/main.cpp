#include "SandboxLayer.h"


using namespace GL;

int main()
{
	Application sandbox;
	ApplicationConfig config;

	sandbox.Init(config);

	sandbox.PushLayer(new SandboxLayer("Render Layer"));

	sandbox.Run();

	return 0;
}


