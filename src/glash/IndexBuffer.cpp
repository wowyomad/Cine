#include "glash/IndexBuffer.hpp"
#include "glash/structures.hpp"
#include "glash/logger.hpp"

namespace glash
{
	IndexBuffer::IndexBuffer(const std::vector<Index>& data, GLBufferUsage usage = GLBufferUsage::STATIC_DRAW)
		
	{
		m_Count = data.size();
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(Index), data.data(), usage));
	}
	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	{
		*this = std::move(other);
	}
	IndexBuffer::~IndexBuffer()
	{
		LOG_INFO("Index Buffer Destructor for id:{} ", m_RendererID);
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
	{
		m_RendererID = other.m_RendererID;
		m_Count = other.m_Count;

		other.m_RendererID = 0;

		return *this;
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

	}

	void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

}
