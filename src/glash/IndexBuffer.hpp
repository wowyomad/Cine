#pragma once
#include "glash/Core/Core.hpp"

#include "glash/glash_pch.hpp"
#include "glash/Enums.hpp"

namespace glash
{
	class GLASH_API IndexBuffer_OLD
	{
	public:
		using Index = GLuint;

		IndexBuffer_OLD() : m_Count(0), m_RendererID(0) { }
		IndexBuffer_OLD(const std::vector<Index>& data, GLBufferUsage usage = GLBufferUsage::STATIC_DRAW);
		IndexBuffer_OLD(const IndexBuffer_OLD& other) = default;
		IndexBuffer_OLD(IndexBuffer_OLD&& other) noexcept;
		~IndexBuffer_OLD();

		IndexBuffer_OLD& operator=(IndexBuffer_OLD& other) = default;
		IndexBuffer_OLD& operator=(IndexBuffer_OLD&& other) noexcept;

		void Bind() const;
		static void Unbind();

		inline Index GetCount() const { return m_Count; }
	private:
		GLuint m_RendererID;
		GLuint m_Count;
	};
}
