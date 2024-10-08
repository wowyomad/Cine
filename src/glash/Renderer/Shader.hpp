#pragma once

#include "glash/Core/Base.hpp"
#include <glm/glm.hpp>

namespace Cine
{
	class CINE_SHADER_CLASS;
	class IShader;

	using Shader = CINE_SHADER_CLASS;

	class GLASH_API IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBool(const std::string& name, bool value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		static Ref<Shader> Create(const std::filesystem::path& filepath);
		Ref<Shader> Create(const std::string& name, const std::filesystem::path& filepath);
		virtual const std::string GetName() const = 0;
	};

	class GLASH_API ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::filesystem::path& filepath);
		Ref<Shader> Load(const std::string& name, const std::filesystem::path& filepath);

		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;

	private:
		std::map<std::string, Ref<Shader>> m_Shaders;
	};
}
//ok that is stupid but it works
#include "glash/Platform/OpenGL/OpenGLShader.hpp"
