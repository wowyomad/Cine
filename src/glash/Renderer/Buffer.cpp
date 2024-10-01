#include "Buffer.hpp"
#include "glash/glash_pch.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/Renderer.hpp"
#include "glash/Platform/OpenGL/OpenGLBuffer.hpp"

namespace glash
{
	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t size)
	{
		return Renderer::Get().CreateIndexBufferFn(indices, size);
	}

	Ref<VertexBuffer> glash::VertexBuffer::Create(const float* vertices, uint32_t count)
	{
		return Renderer::Get().CreateVertexBufferFn(vertices, count);
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return Ref<OpenGLVertexBuffer>();
	}


}