#include "Shader.h"
#include "../Debug/Log.h"


namespace GL {

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	};


	void Shader::Use()
	{
		glUseProgram(m_ProgramID);
	};


	void Shader::Add(ShaderType type, const std::string& filePath)
	{
		m_Shaders[type] = filePath;
	};


	void Shader::Compile()
	{
		unsigned int program = glCreateProgram();

		std::vector<unsigned int> shaderIDs;
		shaderIDs.reserve(m_Shaders.size());

		for (const auto& shaderKV : m_Shaders)
		{
			unsigned int shader = glCreateShader(shaderKV.first);
			const char* src = shaderKV.second.c_str();

			glShaderSource(shader, 1, &src, nullptr);

			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE) {

				int length;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

				char* message = (char*)_malloca(length * sizeof(char)); // create char on stack
				glGetShaderInfoLog(shader, length, &length, message);


				GL_LOG_ERROR("Failed to compile shader: {0}", message);

				glDeleteShader(shader);
			}

			glAttachShader(program, shader);
			shaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

		if (isLinked == GL_FALSE) {
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)_malloca(length * sizeof(char)); // create char on stack
			glGetProgramInfoLog(program, length, &length, message);

			GL_LOG_ERROR("Failed to link program: {0}", message);

			glDeleteProgram(program);

			for (auto& id : shaderIDs)
				glDeleteShader(id);

			return;
		}

		for (auto& id : shaderIDs)
			glDeleteShader(id);

		m_ProgramID = program;

		GL_LOG_INFO("Shader Compile Successfull.");
	};


	std::string Shader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				GL_LOG_ERROR("Could not read from file: {0}", filePath);
			}
		}
		else
		{
			GL_LOG_ERROR("Could not open file: {0}", filePath);
		}

		return result;
	}

}