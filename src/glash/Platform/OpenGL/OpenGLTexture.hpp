#pragma once
#include "glash/Core/Core.hpp"
#if CINE_PLATFORM_OPENGL

#include "glash/Renderer/Texture.hpp"

#include <filesystem>
#include <glad/glad.h>

namespace Cine
{
	class OpenGLTexture2D : public Texture2D 
	{
	public:
		virtual ~OpenGLTexture2D();

		OpenGLTexture2D(const std::filesystem::path& filepath, const TextureSpecification& specification = TextureSpecification());
		OpenGLTexture2D(const TextureSpecification& specification);
		void SetData(void* data, size_t size) override final;

		void Bind(uint32_t slot) const override final;

		const TextureSpecification& GetSpecification() const override final { return m_Specification; }
		uint32_t GetWidth() const override final { return m_Width; }
		uint32_t GetHeight() const override final { return m_Height;  }
		inline uint32_t GetRendererID() const override final { return m_RendererID; }
		virtual bool IsLoaded() const override final { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override final
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		uint32_t m_RendererID;

		TextureSpecification m_Specification;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		std::filesystem::path m_Filepath;
		GLenum m_InternalFormat, m_DataFormat;
	};
}

#endif