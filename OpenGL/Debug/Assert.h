#pragma once

#include "Log.h"


#define GL_ASSERT(check, ...) { if(!(check)) { GL_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
