#pragma once
#include "glash/glash_pch.hpp"

namespace glash
{
		enum SHADER_TYPE
		{
			VERTEX_SHADER = GL_VERTEX_SHADER,
			FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
			COMPUTE_SHADER = GL_COMPUTE_SHADER,
			TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
			TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
			GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
			NONE = -1,
		};

		struct ShaderSource
		{
			enum SHADER_TYPE type;
			std::string source;
		};

		class ShaderProgram
		{
		public:
			class Builder
			{
			public:
				bool AddShader(const std::string& shaderPath, enum SHADER_TYPE type);
				void AddShader(const ShaderSource& shaderSource);
				void AddShaders(const std::vector<ShaderSource>& shaderSources);
				void CleanShaders();
				ShaderProgram Build();

			private:
				std::vector<GLuint> m_Shaders = {};
			};

			static ShaderProgram MakeShaderProgram(const char* vertRelPath, const char* fragRelPath);

			ShaderProgram(GLuint program_id);
			~ShaderProgram();

			void Reset();
			void Use() const;
			void SetUniformVec(const std::string& name, const glm::vec4& value, GLint type);
			
			operator bool() const;
			bool isLinked() const;
		public:



		private:
			GLuint m_Program = 0;
			 
		};	
}