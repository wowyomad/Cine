#include "glash/Platform/OpenGL/OpenGLBuffer.hpp"
#include "glash/glash_pch.hpp"

namespace glash
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t size)
		: m_RendererID(0)
	{
		GLCall(glCreateBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		assert(false && "Not implemented yet");
	}

	void OpenGLVertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	} 

	void OpenGLVertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}


	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count)
		: m_RendererID(0), m_Count(count)
	{
		GLCall(glCreateBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * count, indices, GL_STATIC_DRAW));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void OpenGLIndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
	uint32_t OpenGLIndexBuffer::GetCount() const
	{
		return m_Count;
	}
}