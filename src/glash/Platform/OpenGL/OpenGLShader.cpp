#include "glash/glash_pch.hpp"
#include "glash/Platform/OpenGL/OpenGLShader.hpp"

#include "glash/Core/Log.hpp"
#include "glash/utils/file_reader.hpp"

namespace glash
{
	OpenGLShader::OpenGLShader()
		:m_ProgramID(0)
	{

	}
	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_Path(filepath), m_ProgramID(0)
	{
		Reload();
	}
	OpenGLShader::~OpenGLShader()
	{
		GLCall(glDeleteProgram(m_ProgramID));
	}
	void OpenGLShader::Reload()
	{
		auto shaders = ParseShader(m_Path);

		if (!CreateShaderProgram(shaders)) {
			GLASH_CORE_ERROR("Failed to load shader from {}", m_Path);
		}
	}
	void OpenGLShader::Bind() const
	{
		GLCall(glUseProgram(m_ProgramID));
	}
	void OpenGLShader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	bool OpenGLShader::isLinked() const
	{
		return GLGetStatus(m_ProgramID, GLStatus::PROGRAM_LINK);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		GLenum type = GetUniformType(name.c_str());
		if (type != GL_FLOAT)
		{
			GLASH_CORE_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_FLOAT);
			return;
		}
		GLint location = GetUniformLocation(name.c_str());
		GLCall(glUniform1f(location, value));
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		GLenum type = GetUniformType(name.c_str());
		if (type != GL_FLOAT_VEC2)
		{
			GLASH_CORE_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_FLOAT_VEC2);
			return;
		}
		GLint location = GetUniformLocation(name.c_str());
		GLCall(glUniform2f(location, value.x, value.y));
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		GLenum type = GetUniformType(name.c_str());
		if (type != GL_FLOAT_VEC3)
		{
			GLASH_CORE_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_FLOAT_VEC3);
			return;
		}
		GLint location = GetUniformLocation(name.c_str());
		GLCall(glUniform3f(location, value.x, value.y, value.z));
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		GLenum type = GetUniformType(name.c_str());
		if (type != GL_FLOAT_VEC4)
		{
			GLASH_CORE_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_FLOAT_VEC4);
			return;
		}
		GLint location = GetUniformLocation(name.c_str());
		GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		GLenum type = GetUniformType(name.c_str());
		if (type != GL_INT)
		{
			GLASH_CORE_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_INT);
			return;
		}
		GLint location = GetUniformLocation(name.c_str());
		GLCall(glUniform1i(location, value));
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		assert(false && "Not implemented yet");
	}

	void OpenGLShader::SetBool(const std::string& name, bool value)
	{
		GLenum type = GetUniformType(name.c_str());
		if (type != GL_BOOL)
		{
			GLASH_CORE_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_BOOL);
			return;
		}
		GLint location = GetUniformLocation(name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const float*>(&value));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		GLenum type = GetUniformType(name.c_str());
		if (type != GL_FLOAT_MAT4)
		{
			GLASH_CORE_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_FLOAT_MAT4);
			return;
		}
		GLint location = GetUniformLocation(name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const float*>(&value));
	}

	void OpenGLShader::SetSamplerSlot(const char* name, GLSampler sampler, const int slot)
	{
		GLenum type = GetUniformType(name);
		if (type != sampler)
		{
			GLASH_CORE_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_INT);
			return;
		}
		GLint location = GetUniformLocation(name);
		GLCall(glUniform1i(location, slot));
	}

	inline const std::string OpenGLShader::GetName() const
	{
		return m_Path;
	}

	GLint OpenGLShader::GetUniformLocation(const char* name)
	{
		if (m_UniformLocations.contains(name))
		{
			return m_UniformLocations[name];
		}
		GLint location = glGetUniformLocation(m_ProgramID, name);

		m_UniformLocations[name] = location;

		return location;
	}

	GLenum OpenGLShader::GetUniformType(const char* name)
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

	bool OpenGLShader::CompileShader(const ShaderSource& shaderSource, GLuint& shaderID)
	{
		const char* sourceCode = shaderSource.source.c_str();
		GLCall(glShaderSource(shaderID, 1, &sourceCode, nullptr));
		GLCall(glCompileShader(shaderID));

		bool success = GLGetStatus(shaderID, GLStatus::SHADER_COMPILE);
		if (!success) {
			GLASH_CORE_ERROR("Shader compilation failed for type {}", static_cast<GLuint>(shaderSource.type));
			glDeleteShader(shaderID);
			return false;
		}
		return true;
	}

	bool OpenGLShader::CreateShaderProgram(const std::vector<ShaderSource>& sources)
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

	std::vector<ShaderSource> OpenGLShader::ParseShader(const std::string& filepath)
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
}
