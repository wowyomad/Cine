#pragma once

#include "glash/Renderer/FrameBuffer.hpp"

namespace Cine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& specification);
		~OpenGLFrameBuffer();

		const FramebufferSpecification& GetSpecification() const override { return m_Specification; };
		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
		{
			CINE_CORE_ASSERT(index < m_ColorAttachments.size(), ""); 
			return m_ColorAttachments[index];
		}

		void Resize(uint32_t width, uint32_t height) override;

		void Invalidate();

		//Get rid of this!
		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;

	private:
		void ClearBuffer();

	//Shoulnd't be hardcoded
	static uint32_t s_MaxFrameBufferSize;
	};
}