#include "glash/VertexBuffer_OLD.hpp"
#include "glash/Enums.hpp"
#include "glash/Core/Log.hpp"

namespace glash
{
	VertexBuffer_OLD::VertexBuffer_OLD(VertexBuffer_OLD&& other) noexcept
	{
		*this = std::move(other);
	}
	VertexBuffer_OLD::~VertexBuffer_OLD()
	{
		LOG_INFO("Vertex Buffer Destructor for id:{} ", m_RendererID);
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	VertexBuffer_OLD& VertexBuffer_OLD::operator=(VertexBuffer_OLD&& other) noexcept
	{
		m_RendererID = other.m_RendererID;
		other.m_RendererID = 0;
		return *this;
	}

	void VertexBuffer_OLD::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void VertexBuffer_OLD::Unbind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

}
