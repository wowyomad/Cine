#include "Buffer.hpp"
#include "glash/glash_pch.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/RendererAPI.hpp"
#include "glash/Renderer/Renderer.hpp"

#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Cine
{
	Ref<VertexBuffer> Cine::VertexBuffer::Create(const float* vertices, uint32_t size)
	{
		CINE_PROFILE_FUNCTION();

		return Renderer::Get().CreateVertexBuffer(vertices, size);
	}	

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
	{
		CINE_PROFILE_FUNCTION();

		return Renderer::Get().CreateIndexBuffer(indices, count);
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		CINE_PROFILE_FUNCTION();

		return Renderer::Get().CreateVertexBufferEmpty(size);
	}


}