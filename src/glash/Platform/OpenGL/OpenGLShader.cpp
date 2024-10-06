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

			GLASH_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}
	}

	OpenGLShader::OpenGLShader(const std::filesystem::path& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		m_OpenGLSourceCode = shaderSources;

		CompileOrGetOpenGLBinaries();
		CreateProgram();
		
		m_Name = filepath.filename().string();
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
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform1i(location, value));
	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform1i(location, value));
	}
	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform1iv(location, count, values));
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform1f(location, value));
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform2f(location, value.x, value.y));
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform3f(location, value.x, value.y, value.z));

	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		GLCall(glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix)));
	}
	const std::string OpenGLShader::GetName() const
	{
		return std::string();
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
	std::map<int, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::map<int, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		GLASH_CORE_ASSERT(pos != std::string::npos, "#type not found in shader file {}", source);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			GLASH_CORE_ASSERT(eol != std::string::npos, "Incorrect end of line. CR-LF expected");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			int glType = utils::ShaderTypeFromString(type);
			GLASH_CORE_ASSERT(glType, "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			GLASH_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line 

			shaderSources[glType] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		
	}

	void OpenGLShader::CreateProgram()
	{
		bool success = true;
		GLuint program = glCreateProgram(); // Create the program

		std::vector<GLuint> shaderIDs;

		for (auto&& [type, source] : m_OpenGLSourceCode)
		{
			const char* rawSource = source.c_str();
			GLCall(GLuint shaderID = glCreateShader(type));
			GLCall(glShaderSource(shaderID, 1, &rawSource, nullptr));
			GLCall(glCompileShader(shaderID));

			success = GLGetStatus(shaderID, GL_COMPILE_STATUS);
			if (!success) {
				GLASH_CORE_ERROR("Shader compilation failed for {}", static_cast<GLuint>(type));
				DEBUG_BREAK;
				glDeleteShader(shaderID);
				success = false;
				break;
			}

			GLCall(glAttachShader(program, shaderID));
			shaderIDs.push_back(shaderID); // Store shader ID for later deletion
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