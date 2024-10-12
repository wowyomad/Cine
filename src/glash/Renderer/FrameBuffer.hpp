#pragma once
#include "glash/Core/Base.hpp"

namespace Cine
{
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1; 

		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual const FrameBufferSpecification& GetSpecifiction() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& specification);
	};
}