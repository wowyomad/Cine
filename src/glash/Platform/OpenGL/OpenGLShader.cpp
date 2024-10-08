#include "glash/glash_pch.hpp"
#include "glash/Platform/OpenGL/OpenGLShader.hpp"

namespace Cine
{
	namespace utils
	{
		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			CINE_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}
	}

	static inline int GetUniformLocation(uint32_t rendererID, const char* name)
	{
		GLint location = glGetUniformLocation(rendererID, name);
		CINE_CORE_ASSERT(location != -1, "Uniform {} not found", name);
		return location;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::filesystem::path& filepath)
		: m_RendererID(0)
	{
		m_OpenGLSourceCode = ReadFile(filepath);

		CompileOrGetOpenGLBinaries(); //does nothing
		CreateProgram();
		if (m_RendererID != 0)
		{
			m_FilePath = filepath.string();
			m_Name = name;
		}
	}
	OpenGLShader::~OpenGLShader()
	{
	}
	void OpenGLShader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}
	void OpenGLShader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}
	void OpenGLShader::UploadUniformBool(const std::string& name, bool value)
	{
		GLint location = GetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform1i(location, value));
	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform1i(location, value));
	}
	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = GetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform1iv(location, count, values));
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform1f(location, value));
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform2f(location, value.x, value.y));
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform3f(location, value.x, value.y, value.z));

	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = GetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix)));
	}
	const std::string OpenGLShader::GetName() const
	{
		return m_Name;
	}
	std::string OpenGLShader::ReadFile(const std::filesystem::path& filepath)
	{
		std::string result;

		std::ifstream in(filepath, std::ios::in | std::ios::binary);
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
				GLASH_CORE_ERROR("Could not read file {}", filepath.string());
			}
		}
		else
		{
			GLASH_CORE_ERROR("Could not open file {}", filepath.string());
		}
		return result;
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		
	}

	void OpenGLShader::CreateProgram()
	{
		bool success = true;
		GLuint program = glCreateProgram(); // Create the program

		int shaderIDs[2];
		int types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
		const char* rawSource = m_OpenGLSourceCode.c_str();
		for (int i = 0; i < 2; i++)
		{
			const char* shaderDefine = nullptr;
			if (types[i] == GL_VERTEX_SHADER) {
				shaderDefine = "#version 450 core\n#define VERTEX\n";
			}
			else if (types[i] == GL_FRAGMENT_SHADER) {
				shaderDefine = "#version 450 core\n#define FRAGMENT\n";
			}

			GLCall(GLuint shaderID = glCreateShader(types[i]));

			const char* sources[] = { shaderDefine, rawSource };
			GLCall(glShaderSource(shaderID, 2, sources, nullptr));
			GLCall(glCompileShader(shaderID));

			success = GLGetStatus(shaderID, GL_COMPILE_STATUS);
			if (!success) {
				GLint logLength;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

				if (logLength > 0) {
					std::vector<char> errorLog(logLength);
					glGetShaderInfoLog(shaderID, logLength, nullptr, errorLog.data());
					GLASH_CORE_ERROR("Shader compilation failed: {}", errorLog.data());
				}
				DEBUG_BREAK;
				glDeleteShader(shaderID);
				success = false;
				break;
			}

			GLCall(glAttachShader(program, shaderID));
			shaderIDs[i] = shaderID;
		}

		if (success)
		{
			GLCall(glLinkProgram(program));
			success = GLGetStatus(program, GL_LINK_STATUS);
			if (success) {
				m_RendererID = program;
			}
			else {
				GLASH_CORE_ERROR("Shader linking failed.");
				DEBUG_BREAK;
			}
		}

		for (GLuint shaderID : shaderIDs)
		{
			GLCall(glDetachShader(program, shaderID));
			GLCall(glDeleteShader(shaderID));
		}

		if (!success)
		{
			glDeleteProgram(program);
			m_RendererID = 0;
		}
	}
	void OpenGLShader::SetBool(const std::string& name, bool value)
	{
		UploadUniformBool(name, value);
	}
	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}
	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}
	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}
}