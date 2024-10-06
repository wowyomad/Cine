#pragma once

#include "glash/Renderer/Shader.hpp"
#include <GLM/glm.hpp>

namespace glash
{
	class OpenGLShader : public Shader
	{
	public:

		OpenGLShader(const std::filesystem::path& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override final;
		virtual void Unbind() const override final;

		virtual void SetBool(const std::string& name, bool value) override final;
		virtual void SetInt(const std::string& name, int value) override final;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override final;
		virtual void SetFloat(const std::string& name, float value) override final;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override final;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override final;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override final;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override final;
		
		void UploadUniformBool(const std::string& name, bool value);
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		const std::string GetName() const override final;

	private:
		std::string ReadFile(const std::filesystem::path& filepath);
		std::map<int, std::string> PreProcess(const std::string& source);

		void CompileOrGetOpenGLBinaries();
		void CreateProgram();

	private:
		uint32_t m_RendererID;
		std::string m_FilePath;
		std::string m_Name;
		
		std::map<int, std::string> m_OpenGLSourceCode;
	};
}