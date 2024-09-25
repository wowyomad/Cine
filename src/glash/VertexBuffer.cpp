#include "glash/VertexBuffer.hpp"
#include "glash/Enums.hpp"
#include "glash/Log.hpp"

namespace glash
{
	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	{
		*this = std::move(other);
	}
	VertexBuffer::~VertexBuffer()
	{
		LOG_INFO("Vertex Buffer Destructor for id:{} ", m_RendererID);
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		m_RendererID = other.m_RendererID;
		other.m_RendererID = 0;
		return *this;
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void VertexBuffer::Unbind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

}
