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
		Reload();
		/*auto shaders = ParseShader(filepath);

		ShaderCompiler builder;
		for (const auto& shader : shaders)
		{
			builder.AddShader(shader);
		}

		m_ProgramID = builder.CompileAndLink();*/

	}
	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_ProgramID));
	}
	void Shader::Reload()
	{
		auto shaders = ParseShader(m_FilePath);

		if (!CreateShaderProgram(shaders)) {
			LOG_ERROR("Failed to load shader from {}", m_FilePath);
		}
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

	void Shader::SetSamplerSlot(const char* name, GLSampler sampler, const int slot)
	{
		GLenum type = GetUniformType(name);
		if (type != sampler)
		{
			LOG_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_INT);
			return;
		}
		GLint location = GetUniformLocation(name);
		GLCall(glUniform1i(location, slot));
	}
	
	GLint Shader::GetUniformLocation(const char* name)
	{
		if (m_UniformLocations.contains(name))
		{
			return m_UniformLocations[name];
		}
		GLint location = glGetUniformLocation(m_ProgramID, name);

		m_UniformLocations[name] = location;

		return location;
	}

	GLenum Shader::GetUniformType(const char* name)
	{
		if (m_UniformTypes.contains(name))
		{
			return m_UniformTypes[name];
		}

		GLenum type;
		GLint size;
		GLsizei length;
		GLCall(glGetActiveUniform(m_ProgramID, GetUniformLocation(name), 0, &length, &size, &type, nullptr));
		m_UniformTypes[name] = type;

		return type;
	}

	bool Shader::CompileShader(const ShaderSource& shaderSource, GLuint& shaderID)
	{
		const char* sourceCode = shaderSource.source.c_str();
		GLCall(glShaderSource(shaderID, 1, &sourceCode, nullptr));
		GLCall(glCompileShader(shaderID));

		bool success = GLGetStatus(shaderID, GLStatus::SHADER_COMPILE);
		if (!success) {
			LOG_ERROR("Shader compilation failed for type {}", static_cast<GLuint>(shaderSource.type));
			glDeleteShader(shaderID);
			return false;
		}
		return true;
	}

	bool Shader::CreateShaderProgram(const std::vector<ShaderSource>& sources)
	{
		GLuint programID = glCreateProgram();
		std::vector<GLuint> compiledShaders;

		for (const auto& source : sources) {
			GLuint shaderID;
			GLCall(shaderID = glCreateShader(source.type));
			if (CompileShader(source, shaderID)) {
				GLCall(glAttachShader(programID, shaderID));
				compiledShaders.push_back(shaderID);
			}
			else {
				return false;
			}
		}

		GLCall(glLinkProgram(programID));
		bool success = GLGetStatus(programID, GLStatus::PROGRAM_LINK);
		if (!success) {
			LOG_ERROR("Program linking failed.");
			GLCall(glDeleteProgram(programID));
			return false;
		}

		for (GLuint shaderID : compiledShaders) {
			GLCall(glDeleteShader(shaderID));
		}

		if (m_ProgramID != 0) {
			GLCall(glDeleteProgram(m_ProgramID));
		}
		m_ProgramID = programID;
		return true;
	}

	std::vector<ShaderSource> Shader::ParseShader(const std::string& filepath)
	{
		std::ifstream file(filepath);

		if (!file)
		{
			LOG_ERROR("Coudln't open file {}", filepath);
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
					LOG_ERROR("File {} doesn't begin with #shader <type>", filepath);
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

	GLuint Shader::ShaderCompiler::CompileAndLink(GLuint programID)
	{
		std::map<GLuint, bool> requiredShaders{
			{GLShaderType::VERTEX_SHADER, false},
			{GLShaderType::FRAGMENT_SHADER, false}
		};

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
				return 0;
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
			return 0;
		}

		CleanShaders();

		return programID;
	}

	GLuint Shader::ShaderCompiler::CompileAndLink()
	{
		
		GLuint programID = glCreateProgram();
		GLuint resultID = CompileAndLink(programID);
		if (resultID == 0)
		{
			GLCall(glDeleteProgram(programID));
			return 0;
		}
	}

}