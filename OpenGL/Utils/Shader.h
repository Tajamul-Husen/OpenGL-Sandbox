#pragma once

#include <glad/glad.h>
#include "../GLCommon.h"


namespace GL {

	enum ShaderType {
		VERTEX_SHADER = GL_VERTEX_SHADER,
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
	};


	static std::string GetShaderFromType(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::VERTEX_SHADER: return "Vertex";
		case ShaderType::FRAGMENT_SHADER: return "Fragment";
		case ShaderType::GEOMETRY_SHADER: return "Geometry";
		}
	}


	class ShaderSystem
	{
	public:
		ShaderSystem() = default;
		~ShaderSystem();

		void Add(ShaderType type, const std::string& filePath);
		bool Compile();

		unsigned int GetProgramID() { return m_ProgramID; };

	private:
		std::string ReadFile(const std::string& filePath);

	private:
		std::unordered_map<ShaderType, std::string> m_Shaders;
		unsigned int m_ProgramID;
	};

}

