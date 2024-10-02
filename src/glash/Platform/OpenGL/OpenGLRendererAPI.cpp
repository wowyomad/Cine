#include "glash/glash_pch.hpp"
#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"
#include "glash/Platform/OpenGL/OpenGLVertexArray.hpp"
#include "glash/Platform/OpenGL/OpenGLBuffer.hpp"

namespace glash
{
	Ref<VertexBuffer> OpenGLRendererAPI::CreateVertexBuffer(const float* vertices, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}
	Ref<VertexBuffer> OpenGLRendererAPI::CreateVertexBufferEmpty(uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(size);

	}
	Ref<IndexBuffer> OpenGLRendererAPI::CreateIndexBuffer(const uint32_t* indices, uint32_t size)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, size);

	}
	Ref<VertexArray> OpenGLRendererAPI::CreateVertexArray()
	{
		return CreateRef<OpenGLVertexArray>();
	}

	void glash::OpenGLRendererAPI::Init()
	{

	}
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		GLCall(glViewport(x, y, width, height));
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}
	void OpenGLRendererAPI::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}
	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}