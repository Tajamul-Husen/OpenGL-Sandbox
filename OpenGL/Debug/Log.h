#pragma once

#include <spdlog/spdlog.h>

// Logging macros
#define GL_LOG_INFO(...)          spdlog::info(__VA_ARGS__)
#define GL_LOG_WARN(...)          spdlog::warn(__VA_ARGS__)
#define GL_LOG_ERROR(...)         spdlog::error(__VA_ARGS__)
#define GL_LOG_TRACE(...)		  spdlog::trace(__VA_ARGS__)
#define GL_LOG_CRITICAL(...)	  spdlog::critical(__VA_ARGS__)


