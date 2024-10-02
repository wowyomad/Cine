#include "glash/glash_pch.hpp"
#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"
#include "glash/Platform/OpenGL/OpenGLVertexArray.hpp"
#include "glash/Platform/OpenGL/OpenGLBuffer.hpp"

namespace glash
{
	Scope<OpenGLRendererAPI> RendererAPI::Create()
	{
		return CreateScope<OpenGLRendererAPI>();
	}


	Ref<VertexBuffer> RendererAPI::CreateVertexBuffer(const float* vertices, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}
	Ref<VertexBuffer> RendererAPI::CreateVertexBufferEmpty(uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(size);

	}
	Ref<IndexBuffer> RendererAPI::CreateIndexBuffer(const uint32_t* indices, uint32_t size)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, size);

	}
	Ref<VertexArray> RendererAPI::CreateVertexArray()
	{
		return CreateRef<OpenGLVertexArray>();
	}

	void glash::OpenGLRendererAPI::Init()
	{
	}
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
	}
	void OpenGLRendererAPI::Clear()
	{
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
	}
	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
	}
}