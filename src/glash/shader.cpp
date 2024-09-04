#include "glash/glash_pch.hpp"

#include "shader.hpp"
#include "glash/helper/file_reader.hpp"
#include "glash/logger.hpp"

namespace glash
{

	ShaderProgram ShaderProgram::MakeShaderProgram(const char* vsPath, const char* fsPath)
	{
		Builder builder;
		builder.AddShader(vsPath, SHADER_TYPE::VERTEX_SHADER);
		builder.AddShader(fsPath, SHADER_TYPE::FRAGMENT_SHADER);
		auto program = builder.Build();
		return program;
	}

	ShaderProgram::ShaderProgram(GLuint program_id)
		: m_Program(program_id)
	{

	}
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_Program);
	}

	ShaderProgram::operator bool() const
	{
		return isLinked();
	}

	bool ShaderProgram::isLinked() const
	{
		return GLGetStatus(m_Program, GLStatus::PROGRAM_LINK);
	}

	void ShaderProgram::Use() const
	{
		GLCall(glUseProgram(m_Program));
	}


	void ShaderProgram::Reset()
	{
		GLCall(glUseProgram(0));
	}
	bool ShaderProgram::Builder::AddShader(const std::string& shaderPath, enum SHADER_TYPE type)
	{
		GLuint shader = glCreateShader(type);
		std::unique_ptr<char[]> data = glash::ReadFile(shaderPath);
		const char* rawData = data.get();

		if (!data)
		{
			LOG_ERROR("Trying to add non-existing shader file {}", shaderPath);
			GLCall(glDeleteShader(shader));
			return false;
		}

		GLCall(glShaderSource(shader, 1, &rawData, nullptr));

		m_Shaders.push_back(shader);
		return true;
	}
	void ShaderProgram::Builder::AddShader(const ShaderSource& shaderSource)
	{
		const char* rawData = shaderSource.source.c_str();

		GLuint shader = glCreateShader(shaderSource.type);
		GLCall(glShaderSource(shader, 1, &rawData, nullptr));

		m_Shaders.push_back(shader);
	}
	void ShaderProgram::Builder::AddShaders(const std::vector<ShaderSource>& shaderSources)
	{
		for (const ShaderSource& shaderSource : shaderSources)
		{
			AddShader(shaderSource);
		}
	}
	void ShaderProgram::Builder::CleanShaders()
	{
		for (GLuint shader : m_Shaders)
		{
			glDeleteShader(shader);
		}
		m_Shaders.clear();
	}
	ShaderProgram ShaderProgram::ShaderProgram::Builder::Build()
	{
		std::map<GLuint, bool> requiredShaders{
			{SHADER_TYPE::VERTEX_SHADER, false},
			{SHADER_TYPE::FRAGMENT_SHADER, false}
		};
		GLuint program = glCreateProgram();
		bool success = true;
		for (GLuint shader : m_Shaders)
		{
			GLint shaderType = GLGetStatus(shader, GLStatus::SHADER_TYPE);
			if (!requiredShaders.contains(shaderType))
			{
				LOG_WARN("Tried to add unsupported shader type {}", shaderType);
				continue;
			}

			GLCall(glCompileShader(shader));
			GLCall(glAttachShader(program, shader));
			requiredShaders[shaderType] = true;
		}

		for (auto [shaderType, present] : requiredShaders)
		{
			if (!present)
			{
				LOG_WARN("Program doesn't contain required shader of type {}", shaderType);
				success = false;
				break;
			}
		}
		
		if (!success)
		{
			return ShaderProgram(0);
		}

		GLCall(glLinkProgram(program));

		success = GLGetStatus(program, GLStatus::PROGRAM_LINK);
		if (!success)
		{
			LOG_ERROR("Program link failed");
		}

		CleanShaders();

		return ShaderProgram(program);
	}

}