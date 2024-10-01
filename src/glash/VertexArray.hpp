#pragma once

#include "glash/Core/Core.hpp"
#include "glash/glash_pch.hpp"
#include "glash/VertexBuffer_OLD.hpp"
#include "glash/VertexBufferLayout.hpp"

namespace glash
{
	class GLASH_API VertexArray_OLD
	{
	public:
		VertexArray_OLD();
		VertexArray_OLD(const VertexArray_OLD& other) = default;
		VertexArray_OLD(VertexArray_OLD&& other);
		~VertexArray_OLD();

		VertexArray_OLD& operator=(const VertexArray_OLD& other) = default;
		VertexArray_OLD& operator=(VertexArray_OLD&& other) noexcept;

		void AddBuffer(const VertexBuffer_OLD& vb, const VertexBufferLayout_OLD& layout);

		void Bind() const;
		static void Unbind();

	private:
		uint32_t m_RendererID;

	};
}

