#include "glash/glash_pch.hpp"
#include "glash/Renderer/Shader.hpp"

namespace Cine
{
	Ref<Shader> IShader::Create(const std::string& filepath)
	{
		//TODO: Choose correct render API
		return CreateRef<OpenGLShader>(filepath);
	}
}