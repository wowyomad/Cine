#include "glash/glash_pch.hpp"
#include "glash/Renderer/Shader.hpp"

namespace Cine
{
	Ref<Shader> IShader::Create(const std::filesystem::path& filepath)
	{
		//TODO: Choose correct render API
		return CreateRef<OpenGLShader>(filepath.stem().string(), filepath);
	}
	Ref<Shader> IShader::Create(const std::string& name, const std::filesystem::path& filepath)
	{
		//TODO: Choose correct render API
		return CreateRef<OpenGLShader>(name, filepath);
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		CINE_CORE_ASSERT(!Exists(name), "Shader with name {} already in library", name);
		m_Shaders[name] = shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::filesystem::path& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::filesystem::path& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		CINE_CORE_ASSERT(Exists(name), "Shader with name {} not found", name);
		return m_Shaders[name];
	}
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		auto it = m_Shaders.find(name);
		if (it != m_Shaders.end())
		{
			return true;
		}
		return false;
	}
}