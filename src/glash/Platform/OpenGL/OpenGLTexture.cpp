#include "glash/glash_pch.hpp"
#include "glash/Platform/OpenGL/OpenGLTexture.hpp"

#include <stb_image.h>

namespace Cine
{
	namespace Utils
	{
		static GLenum CineImageFormatToGLDataFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB8: return GL_RGB;
			case TextureFormat::RGBA8: return GL_RGBA;
			}
			CINE_CORE_ASSERT(false, "Image format not supported");
			return 0;
		}

		static GLenum CineImageFormatToGLInternalFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB8:  return GL_RGB8;
			case TextureFormat::RGBA8: return GL_RGBA8;
			}

			CINE_CORE_ASSERT(false, "Image format not supported");
			return 0;
		}
		static GLenum CineTextureWrapToGL(TextureWrap wrap)
		{
			switch (wrap)
			{
			case TextureWrap::Repeat: return GL_REPEAT;
			case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
			case TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
			case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
			}
			return GL_REPEAT;
		}

		static GLenum CineTextureFilterToGL(TextureFilter filter)
		{
			switch (filter)
			{
			case TextureFilter::Linear: return GL_LINEAR;
			case TextureFilter::Nearest: return GL_NEAREST;
			}
			return GL_LINEAR;
		}
		static uint32_t CalculateMipMapLevels(uint32_t width, uint32_t height) {
			return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filepath, const TextureSpecification& specification)
		: m_Filepath(filepath), m_Specification(specification)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = nullptr;
		data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
		if (stbi_failure_reason())
		{
			CINE_CORE_ASSERT(false, "stbi failed loading image: {}", stbi_failure_reason());
		}

		if (data)
		{
			m_IsLoaded = true;
			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0;
			GLenum dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			CINE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID,
				m_Specification.GenerateMips ? Utils::CalculateMipMapLevels(m_Width, m_Height) : 1,
				internalFormat,
				m_Width,
				m_Height);

			if (m_Specification.GenerateMips) {
				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else {
				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::CineTextureFilterToGL(m_Specification.MagFilter));

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::CineTextureWrapToGL(m_Specification.Wrap));
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::CineTextureWrapToGL(m_Specification.Wrap));

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			if (m_Specification.GenerateMips) {
				glGenerateTextureMipmap(m_RendererID);
			}

			stbi_image_free(data);
		}
	}
	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		m_InternalFormat = Utils::CineImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::CineImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		glTextureStorage2D(m_RendererID,
			m_Specification.GenerateMips ? Utils::CalculateMipMapLevels(m_Width, m_Height) : 1,
			m_InternalFormat,
			m_Width,
			m_Height);

		if (m_Specification.GenerateMips) {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, Utils::CineTextureFilterToGL(m_Specification.MinFilter));
		}
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, Utils::CineTextureFilterToGL(m_Specification.MagFilter));

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, Utils::CineTextureWrapToGL(m_Specification.Wrap));
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, Utils::CineTextureWrapToGL(m_Specification.Wrap));

		if (m_Specification.GenerateMips) {
			glGenerateTextureMipmap(m_RendererID);
		}
	}

	void OpenGLTexture2D::SetData(void* data, size_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		CINE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}