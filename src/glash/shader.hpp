#pragma once

#include "glash/glash_pch.hpp"
#include "glash/logger.hpp"

namespace glash
{
	namespace fs = std::filesystem;

	class Shader
	{
	public:
		Shader();
		Shader(const std::string& filepath);
		~Shader();

		template <class T>
		void SetUniform(const char* name, const T value)
		{
			static_assert(false, "Uniform of type {} is not supported");
		}

		template <>
		void SetUniform(const char* name, const float value)
		{
			GLenum type = GetUniformType(name);
			if (type != GL_FLOAT)
			{
				LOG_ERROR("Received type: {}. Expected type {}.", type, GL_FLOAT);
				return;
			}
			GLint location = GetUniformLocation(name);
			GLCall(glUniform1f(location, value));
		}

		template <>
		void SetUniform(const char* name, const glm::vec4& value)
		{
			GLenum type = GetUniformType(name);
			if (type != GL_FLOAT)
			{
				LOG_ERROR("Received type: {}. Expected type {}.", type, GL_FLOAT_VEC4);
				return;
			}
			GLint location = GetUniformLocation(name);
			GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
		}

		void Bind() const;
		void Unbind() const;


		operator bool() const;

		class ShaderCompiler
		{
		public:
			void AddShader(const ShaderSource& shaderSource);
			void CleanShaders();
			GLuint CompileAndLink();

		private:
			std::vector<GLuint> m_Shaders = {};
		};
	private:
		GLuint m_ProgramID;
		std::unordered_map<std::string, GLint> uniformLocations;
		std::unordered_map<std::string, GLenum> uniformTypes;

		bool CompileShader();
		bool CreateShader();
		static std::vector<ShaderSource> ParseShader(const fs::path& filepath);

		GLint GetUniformLocation(const char* name);
		GLenum GetUniformType(const char* name);

	private:
		fs::path m_FilePath;
	};
}