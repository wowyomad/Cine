#include "glash/IndexBuffer.hpp"
#include "glash/Enums.hpp"
#include "glash/Core/Log.hpp"

namespace glash
{
	IndexBuffer_OLD::IndexBuffer_OLD(const std::vector<Index>& data, GLBufferUsage usage)
		: IndexBuffer_OLD()
	{
		m_Count = data.size();
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(Index), data.data(), usage));
	}

	IndexBuffer_OLD::IndexBuffer_OLD(IndexBuffer_OLD&& other) noexcept
	{
		*this = std::move(other);
	}
	IndexBuffer_OLD::~IndexBuffer_OLD()
	{
		LOG_INFO("Index Buffer Destructor with RendererID : {} ", m_RendererID);
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	IndexBuffer_OLD& IndexBuffer_OLD::operator=(IndexBuffer_OLD&& other) noexcept
	{
		m_RendererID = other.m_RendererID;
		m_Count = other.m_Count;

		other.m_RendererID = 0;
		other.m_Count = 0;

		return *this;
	}

	void IndexBuffer_OLD::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void IndexBuffer_OLD::Unbind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

}
