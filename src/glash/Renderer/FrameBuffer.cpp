#include "glash/glash_pch.hpp"
#include "glash/Renderer/FrameBuffer.hpp"

#include "glash/Renderer/RendererAPI.hpp"

namespace Cine
{
	Ref<FrameBuffer> FrameBuffer::Create(const FramebufferSpecification& specification)
	{
		return RendererAPI::Get()->CreateFrameBuffer(specification);
	}
}