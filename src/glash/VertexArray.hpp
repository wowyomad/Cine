#pragma once

#include "glash/glash_pch.hpp"
#include "glash/VertexBuffer.hpp"
#include "glash/VertexBufferLayout.hpp"

namespace glash
{
	class VertexArray
	{
	public:
		VertexArray() : m_RendererID(0) 
		{
			GLCall(glGenVertexArrays(1, &m_RendererID));
		}
		VertexArray(const VertexArray& other) = default;
		VertexArray(VertexArray&& other);
		~VertexArray();

		VertexArray& operator=(const VertexArray& other) = default;
		VertexArray& operator=(VertexArray&& other) noexcept;

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		static void Unbind();

	private:
		uint32_t m_RendererID;

	};
}

