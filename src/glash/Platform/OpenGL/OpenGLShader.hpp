#pragma once
#include "glash/Core/Core.hpp"
#if GLASH_PLATFORM_OPENGL

#include "glash/Renderer/Shader.hpp"

namespace glash
{
	class GLASH_API OpenGLShader : public IShader
	{
	public:
		OpenGLShader();
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		void Reload();

		void Bind() const override final;
		void Unbind() const override final;

		inline operator bool() const { return isLinked(); }
		bool isLinked() const;

		void SetBool(const std::string& name, bool bValue) override final;
		void SetInt(const std::string& name, int iValue) override final;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override final;
		void SetFloat(const std::string& name, float fValue) override final;
		void SetFloat2(const std::string& name, const glm::vec2& value) override final;
		void SetFloat3(const std::string& name, const glm::vec3& value) override final;
		void SetFloat4(const std::string& name, const glm::vec4& value) override final;
		void SetMat4(const std::string& name, const glm::mat4& value) override final;

		void SetSamplerSlot(const char* name, uint32_t sampler, const int slot); //Shouldn't be here...

		inline const std::string GetName() const override final;

		//Temp...
		OpenGLShader& operator=(OpenGLShader&& other) noexcept
		{
			this->m_Path = other.m_Path;
			this->m_ProgramID = other.m_ProgramID;
			this->m_UniformLocations = std::move(other.m_UniformLocations);
			this->m_UniformTypes = std::move(other.m_UniformTypes);

			other.m_ProgramID = 0;
			return *this;
		}

	private:
		enum GLShaderType : uint32_t
		{
			None = 0,
			VERTEX_SHADER = GL_VERTEX_SHADER,
			FRAGMENT_SHADER = GL_FRAGMENT_SHADER
		};

		struct ShaderSource
		{
			enum GLShaderType type;
			std::string source;
		};

		bool CompileShader(const ShaderSource& shaderSource, GLuint& shaderID);
		bool CreateShaderProgram(const std::vector<ShaderSource>& sources);
		static std::vector<ShaderSource> ParseShader(const std::string& filepath);

		GLint GetUniformLocation(const char* name);
		GLenum GetUniformType(const char* name);

	private:
		std::string m_Path;
		uint32_t m_ProgramID;
		std::map<std::string, GLint> m_UniformLocations;
		std::map<std::string, GLenum> m_UniformTypes;
	};
}


#endif // GLASH_PLATFORM_OPENGL