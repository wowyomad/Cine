#pragma once

#include "glash/glash_pch.hpp"
#include "glash/logger.hpp"

namespace glash
{
	class Shader
	{
	public:
		Shader();
		Shader(const std::string& filepath);
		~Shader();

		void Reload();

		void Bind() const;
		void Unbind() const;

		operator bool() const;

		template <class T>
		void SetUniform(const char* name, const T value)
		{
			static_assert(false, "Uniform of type {:0X} is not supported");
		}
		template <>
		void SetUniform(const char* name, const float value);
		template <>
		void SetUniform(const char* name, const glm::vec4& value);
		template <>
		void SetUniform(const char* name, const int value);

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
	template<>
	inline void Shader::SetUniform(const char* name, const float value)
	{
		GLenum type = GetUniformType(name);
		if (type != GL_FLOAT)
		{
			LOG_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_FLOAT);
			return;
		}
		GLint location = GetUniformLocation(name);
		GLCall(glUniform1f(location, value));
	}
	template<>
	inline void Shader::SetUniform(const char* name, const glm::vec4& value)
	{
		GLenum type = GetUniformType(name);
		if (type != GL_FLOAT_VEC4)
		{
			LOG_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_FLOAT_VEC4);
			return;
		}
		GLint location = GetUniformLocation(name);
		GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
	}
	template<>
	inline void Shader::SetUniform(const char* name, const int value)
	{
		GLenum type = GetUniformType(name);
		if (type != GL_INT)
		{
			LOG_ERROR("Received type: {:0X}. Expected type {:0X}.", type, GL_INT);
			return;
		}
		GLint location = GetUniformLocation(name);
		GLCall(glUniform1i(location, value));
	}
}