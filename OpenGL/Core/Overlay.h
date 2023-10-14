#pragma once


namespace GL
{
	class Overlay
	{
	public:
		void Init(void* window, const char* glslVersion);
		void Destroy();

		void Begin();
		void End();
	};
};
