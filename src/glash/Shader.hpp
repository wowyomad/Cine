#pragma once
#include "glash/Core/Core.hpp"
#include "glash/Enums.hpp"
#include "glash/glash_pch.hpp"

namespace glash
{
	class GLASH_API Shader
	{
	public:
		Shader();
		Shader(const std::string& filepath);
		~Shader();

		void Reload();

		void Bind() const;
		void Unbind() const;

		inline operator bool() const { return isLinked(); }
		bool isLinked() const;
		
		void SetUniform(const char* name, const float fValue);
		void SetUniform(const char* name, const int iValue);
		void SetUniform(const char* name, const bool bValue);
		void SetUniform(const char* name, const glm::vec4& v4Value);
		void SetUniform(const char* name, const glm::mat4& m4Value);

		void SetSamplerSlot(const char* name, GLSampler sampler, const int slot);

	private:
		bool CompileShader(const ShaderSource& shaderSource, GLuint& shaderID);
		bool CreateShaderProgram(const std::vector<ShaderSource>& sources);
		static std::vector<ShaderSource> ParseShader(const std::string& filepath);

		GLint GetUniformLocation(const char* name);
		GLenum GetUniformType(const char* name);

	private:
		std::string m_Path;
		GLuint m_ProgramID;
		std::unordered_map<std::string, GLint> m_UniformLocations;
		std::unordered_map<std::string, GLenum> m_UniformTypes;
	};
}
