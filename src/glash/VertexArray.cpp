#include "VertexArray.hpp"
#include "logger.hpp"

namespace glash
{
	VertexArray::VertexArray(VertexArray&& other)
	{
		*this = std::move(other);
	}
	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		m_RendererID = other.m_RendererID;
		other.m_RendererID = 0;
		return *this;
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		const uint32_t stride = layout.GetStride();
		uint32_t offset = 0;
		for (size_t i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (void*)(offset));
			offset += element.size * element.count;
		}
		Unbind();
		vb.Unbind();
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::Unbind()
	{
		GLCall(glBindVertexArray(0));
	}

}

