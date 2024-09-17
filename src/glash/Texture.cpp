#include "glash/Texture.hpp"
#include "glash/glash_pch.hpp"

#include "stb_image.h"

#include "glash/logger.hpp"

namespace glash
{
	Texture::Texture()
		: Texture("")
	{
	}
	Texture::Texture(const std::string& path)
		: m_RendererID(0), m_Path(), m_LocalBuffer(nullptr), m_Bits(0), m_Height(0), m_Width(0)
	{
		LoadTexture(path);
	}
	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}
	void Texture::LoadTexture(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Bits, 4);
		if (!buffer)
		{
			LOG_ERROR("Couldn't load texture {}", path);
		}

		m_LocalBuffer = std::unique_ptr<unsigned char[], StbiImageDeleter>(buffer);
		m_Path = path;

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer.get()));
	}
	void Texture::Bind(const GLuint slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}
	void Texture::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	void Texture::StbiImageDeleter::operator()(unsigned char* image)
	{
		stbi_image_free(image);
		LOG_INFO("Cleared image");
	}
}
