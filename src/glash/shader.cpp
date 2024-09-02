#include "glash/glash_pch.hpp"

#include "shader.hpp"
#include "glash/helper/file_reader.hpp"
#include "glash/logger.hpp"

namespace glash
{
	namespace shader
	{
		GLuint MakeModule(const char* shaderRelPath, SHADER_TYPE type)
		{
			try {
				std::unique_ptr<char[]> module_source = glash::ReadFile(shaderRelPath);
				const char* raw_shader = module_source.get();

				GLuint module = glCreateShader(type);
				glShaderSource(module, 1, &raw_shader, nullptr);
				GlCall(glCompileShader(module));
				glCompileShader(module);
				int success;
				glGetShaderiv(module, GL_COMPILE_STATUS, &success);

				if (!success)
				{
					char errorLog[1024];
					glGetShaderInfoLog(module, sizeof(errorLog), nullptr, errorLog);
					fmt::println("MakeShader:\n{}", errorLog);
					DestroyShader(module);
					return 0;
				}
				return module;
			}
			catch (const std::runtime_error& e) {
				fmt::println("{}", e.what());
				return 0;
			}

		}

		GLuint MakeShader(const char* vsPath, const char* fsPath)
		{
			std::array<GLuint, 2> modules
			{
				MakeModule(vsPath, SHADER_TYPE::VERTEX_SHADER),
				MakeModule(fsPath, SHADER_TYPE::FRAGMENT_SHADER)
			};

			if (!modules[0] || !modules[1])
			{
				return 0;
			}

			GLuint shader = glCreateProgram();
			for (GLuint module : modules)
			{
				glAttachShader(shader, module);
			}
			glLinkProgram(shader);

			int success;
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				char errorLog[1024];
				glGetProgramInfoLog(shader, sizeof(errorLog), nullptr, errorLog);
				fmt::println("MakeProgram:\n{}", errorLog);
				DestroyProgram(shader);
				return 0;
			}

			for (GLuint module : modules)
			{
				DestroyShader(module);
			}

			return shader;
		}
		void DestroyShader(GLuint shader)
		{
			glDeleteShader(shader);
		}
		void DestroyProgram(GLuint m_uiProgram)
		{
			glDeleteProgram(m_uiProgram);
		}
		Shader::Shader(const std::string& vertPath, const std::string& fragPath)
		{
			std::array<GLuint, 2> modules;
			{
				MakeModule(vertPath, SHADER_TYPE::VERTEX_SHADER);
				MakeModule(fragPath, SHADER_TYPE::FRAGMENT_SHADER);
			}

			if (!modules[0] || !modules[1])
			{
				throw std::runtime_error("Shader compilation failed");
			}

			m_uiProgram = glCreateProgram();
			for (GLuint module : modules)
			{
				glAttachShader(m_uiProgram, module);
			}
			glLinkProgram(m_uiProgram);

			
		}
		Shader::~Shader()
		{
		}
		GLuint Shader::GetProgram() const
		{
			return GLuint();
		}
		GLuint Shader::MakeModule(const std::string& shaderPath, SHADER_TYPE type)
		{
			return GLuint();
		}
		void Shader::Cleanup()
		{
		}
	}
}