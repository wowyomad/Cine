#pragma once
#include "glash/glash_pch.hpp"

namespace glash
{
	namespace shader
	{
		enum SHADER_TYPE
		{
			VERTEX_SHADER = GL_VERTEX_SHADER,
			FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
			COMPUTE_SHADER = GL_COMPUTE_SHADER,
			TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
			TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
			GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
		};

		class Shader
		{
			Shader(const std::string& vertPath, const std::string& fragPath);
			~Shader();

			GLuint GetProgram() const;

		private:
			GLuint m_uiProgram = 0;

			static GLuint MakeModule(const std::string& shaderPath, SHADER_TYPE type);
			void Cleanup();
		};
		
		GLuint MakeModule(const char* relPath, SHADER_TYPE type);
		GLuint MakeShader(const char* vertRelPath, const char* fragRelPath);
		void DestroyShader(GLuint shader);
		void DestroyProgram(GLuint m_uiProgram);
	}
}