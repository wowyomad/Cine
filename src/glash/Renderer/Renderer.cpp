#include "glash/glash_pch.hpp"
#include "glash/Renderer/Renderer.hpp"

#include "glash/Platform/OpenGL/OpenGLBuffer.hpp"

namespace glash
{
	Renderer* Renderer::s_Instance = nullptr;


	static inline Ref<OpenGLVertexBuffer> OpenGL_CreateVertexBuffer(const float* vertices, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}

	static inline Ref<OpenGLIndexBuffer> OpenGL_CreateIndexBufferr(const uint32_t* indices, uint32_t count)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, count);
	}
	static inline Ref<OpenGLVertexBuffer> OpenGL_CreateVertexBufferEmpty(uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(size);
	}

	Renderer::Renderer(RendererAPI api)
	{
		GLASH_ASSERT(!s_Instance, "Currently only once renderer instance supported");
		s_Instance = this;

		switch (api)
		{
		case RendererAPI::OpenGL:
			CreateVertexBufferFn = OpenGL_CreateVertexBuffer;
			CreateIndexBufferFn = OpenGL_CreateIndexBufferr;
			CreateVertexBufferEmptyFn = OpenGL_CreateVertexBufferEmpty;
			break;
		case RendererAPI::None:
			GLASH_CORE_ASSERT(false, "Shouldn't reach here");
			break;
		}
	}
	
	const Renderer& Renderer::Get()
	{
		return *s_Instance;
	}


	
}