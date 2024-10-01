#include "glash/glash_pch.hpp"
#include "glash/Renderer/Shader.hpp"

#include "glash/Platform/OpenGL/OpenGLShader.hpp"

namespace glash
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		//TODO: Choose correct render API

		return CreateRef<OpenGLShader>(filepath);
	}
}