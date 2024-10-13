#pragma once

#include "glash/Renderer/FrameBuffer.hpp"

namespace Cine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		~OpenGLFrameBuffer();

		const FrameBufferSpecification& GetSpecifiction() const override { return m_Specification; };
		uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		void Resize(uint32_t width, uint32_t height) override;

		void Invalidate();

		//Get rid of this!
		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FrameBufferSpecification m_Specification;

	private:
		void ClearBuffer();

	//Shoulnd't be hardcoded
	static uint32_t s_MaxFrameBufferSize;
	};
}