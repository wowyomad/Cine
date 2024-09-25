#pragma once

#include "glash/Core.hpp"
#include "glash/glash_pch.hpp"
#include "glash/VertexBuffer.hpp"
#include "glash/VertexBufferLayout.hpp"

namespace glash
{
	class GLASH_API VertexArray
	{
	public:
		VertexArray();
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

