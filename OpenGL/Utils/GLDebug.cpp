
#include "GLDebug.h"
#include "../Debug/Log.h"


namespace GL
{
	void APIENTRY messageCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			GL_LOG_CRITICAL(message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:			GL_LOG_ERROR(message); break;
		case GL_DEBUG_SEVERITY_LOW:				GL_LOG_WARN(message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	GL_LOG_TRACE(message); break;
		}

	}

	void EnableDebugging()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(messageCallback, 0);
	}
};
