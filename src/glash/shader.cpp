#include "glash/Shader.hpp"

#include "glash/logger.hpp"
#include "glash/helper/file_reader.hpp"

namespace glash
{
	Shader::Shader()
		:m_ProgramID(0)
	{
	}
	Shader::Shader(const std::string& filepath)
		: m_FilePath(filepath), m_ProgramID(0)
	{
		auto shaders = ParseShader(filepath);
		ShaderCompiler builder;
		for (const auto& shader : shaders)
		{
			builder.AddShader(shader);
		}

		m_ProgramID = builder.CompileAndLink();

	}
	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_ProgramID));
	}
	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_ProgramID));
	}
	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}
	Shader::operator bool() const
	{
		return GLGetStatus(m_ProgramID, GLStatus::PROGRAM_LINK);
	}
	bool Shader::CompileShader()
	{
		return false;
	}
	bool Shader::CreateShader()
	{
		return false;
	}
	GLint Shader::GetUniformLocation(const char* name)
	{
		if (uniformLocations.contains(name))
		{
			return uniformLocations[name];
		}
		GLint location = glGetUniformLocation(m_ProgramID, name);

		uniformLocations[name] = location;

		return location;
	}

	GLenum Shader::GetUniformType(const char* name)
	{
		if (uniformTypes.contains(name))
		{
			return uniformTypes[name];
		}

		GLenum type;
		GLint size;
		GLsizei length;
		GLCall(glGetActiveUniform(m_ProgramID, GetUniformLocation(name), 0, &length, &size, &type, nullptr));
		uniformTypes[name] = type;

		return type;
	}

	std::vector<ShaderSource> Shader::ParseShader(const fs::path& filepath)
	{
		std::ifstream file(filepath);

		if (!file)
		{
			const std::string message = "Couldn't open file: " + filepath.string();
			LOG_ERROR(fmt::runtime(message));
			return {};
		}

		std::string line;
		std::stringstream vertexShaderStream, fragmentShaderStream;
		enum GLShaderType type = GLShaderType::NONE;

		while (getline(file, line))
		{
			if (line.empty()) continue;
			if (line.contains("#shader"))
			{
				if (line.contains("vertex"))
				{
					type = GLShaderType::VERTEX_SHADER;
				}
				else if (line.contains("fragment"))
				{
					type = GLShaderType::FRAGMENT_SHADER;
				}
			}
			else
			{
				if (type == GLShaderType::VERTEX_SHADER)
				{
					vertexShaderStream << line << '\n';
				}
				else if (type == GLShaderType::FRAGMENT_SHADER)
				{
					fragmentShaderStream << line << '\n';
				}
				else
				{
					LOG_ERROR("File {} doesn't begin with #shader <type>", filepath.string());
					return {};
				}
			}
		}

		std::vector<ShaderSource> sources
		{
			{
			GLShaderType::VERTEX_SHADER,
			vertexShaderStream.str()
			},
			{
			GLShaderType::FRAGMENT_SHADER,
			fragmentShaderStream.str()
			}

		};
		return sources;
	}


	void Shader::ShaderCompiler::AddShader(const ShaderSource& source)
	{
		const char* rawData = source.source.c_str();

		GLuint shader = glCreateShader(source.type);
		GLCall(glShaderSource(shader, 1, &rawData, nullptr));

		m_Shaders.push_back(shader);
	}

	void Shader::ShaderCompiler::CleanShaders()
	{
		for (GLuint shader : m_Shaders)
		{
			glDeleteShader(shader);
		}
		m_Shaders.clear();
	}


	GLuint Shader::ShaderCompiler::CompileAndLink()
	{
		std::map<GLuint, bool> requiredShaders{
			{GLShaderType::VERTEX_SHADER, false},
			{GLShaderType::FRAGMENT_SHADER, false}
		};
		GLuint programID = glCreateProgram();
		bool success = true;
		for (GLuint shader : m_Shaders)
		{
			GLint shaderType = GLGetStatus(shader, GLStatus::GLShaderType);
			if (!requiredShaders.contains(shaderType))
			{
				LOG_WARN("Tried to add unsupported shader type {}", shaderType);
				continue;
			}

			GLCall(glCompileShader(shader));


			GLCall(success = GLGetStatus(shader, GLStatus::SHADER_COMPILE));
			if (!success)
			{
				LOG_ERROR("Shader {} didn't compile", shader);
			}


			GLCall(glAttachShader(programID, shader));
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
			return 0;
		}

		GLCall(glLinkProgram(programID));

		success = GLGetStatus(programID, GLStatus::PROGRAM_LINK);
		if (!success)
		{
			LOG_ERROR("Program link failed");
		}

		CleanShaders();

		return programID;
	}

}