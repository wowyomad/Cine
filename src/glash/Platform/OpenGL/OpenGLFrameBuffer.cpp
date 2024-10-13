#include "glash/glash_pch.hpp"
#include "glash/Platform/OpenGL/OpenGLFrameBufer.hpp"

#include "glash/Renderer/RendererAPI.hpp"

namespace Cine
{
	//Shouldn't be hardcoded, though
	uint32_t OpenGLFrameBuffer::s_MaxFrameBufferSize = 16384;

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& specification)
		: m_Specification(specification)
	{
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		ClearBuffer();
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			CINE_CORE_DEBUG("Tried to set framebuffer to {}x{}", width, height);
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			ClearBuffer();
		}

		GLCall(glCreateFramebuffers(1, &m_RendererID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
		GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));
		 
		CINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is broken");

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::ClearBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);

		m_RendererID = 0;
		m_ColorAttachment = 0;
		m_DepthAttachment = 0;
	}
}
