#pragma once

#include <glad/glad.h>


namespace GL
{
	void APIENTRY messageCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
	void EnableDebugging();
};
