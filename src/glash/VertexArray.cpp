#include "VertexArray.hpp"
#include "glash/Core/Log.hpp"

namespace glash
{

	VertexArray_OLD::VertexArray_OLD()
		: m_RendererID(0)
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}
	VertexArray_OLD::VertexArray_OLD(VertexArray_OLD&& other)
	{
		*this = std::move(other);
	}
	VertexArray_OLD::~VertexArray_OLD()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	VertexArray_OLD& VertexArray_OLD::operator=(VertexArray_OLD&& other) noexcept
	{
		m_RendererID = other.m_RendererID;
		other.m_RendererID = 0;
		return *this;
	}

	void VertexArray_OLD::AddBuffer(const VertexBuffer_OLD& vb, const VertexBufferLayout_OLD& layout)
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
			glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, reinterpret_cast<void*>(offset));
			offset += element.size * element.count;
		}
		Unbind();
		vb.Unbind();
	}

	void VertexArray_OLD::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray_OLD::Unbind()
	{
		GLCall(glBindVertexArray(0));
	}

}

