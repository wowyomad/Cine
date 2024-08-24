#pragma once

#include <GL/glew.h>

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
		GLuint MakeModule(const char* relPath, SHADER_TYPE type);
		GLuint MakeShader(const char* vertexShaderPath, const char* fragmnetShaderPath);
		void DestroyShader(GLuint shader);

	}
}