#pragma once

#include "glash/glash_pch.hpp"
#include "glash/structures.hpp"

namespace glash
{
	class IndexBuffer
	{
	public:
		using Index = GLuint;


		IndexBuffer() : m_Count(0), m_RendererID(0) { }
		IndexBuffer(const std::vector<Index>& data, GLBufferUsage usage = GLBufferUsage::STATIC_DRAW);
		IndexBuffer(const IndexBuffer& other) = default;
		IndexBuffer(IndexBuffer&& other) noexcept;
		~IndexBuffer();

		IndexBuffer& operator=(IndexBuffer& other) = default;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;

		void Bind() const;
		static void Unbind();

		inline Index GetCount() const { return m_Count; }
	private:
		GLuint m_RendererID;
		GLuint m_Count;
	};
}
