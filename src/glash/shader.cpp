#include "shader.hpp"
#include "glash/helper/file_reader.hpp"

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

		GLuint MakeShader(const char* vertRelPath, const char* fragRelPath)
		{
			std::array<GLuint, 2> modules
			{
				MakeModule(vertRelPath, SHADER_TYPE::VERTEX_SHADER),
				MakeModule(fragRelPath, SHADER_TYPE::FRAGMENT_SHADER)
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
		void DestroyProgram(GLuint program)
		{
			glDeleteProgram(program);
		}
	}
}