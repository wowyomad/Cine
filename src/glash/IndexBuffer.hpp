#pragma once

#include "glash/glash_pch.hpp"
#include "glash/structures.hpp"

namespace glash
{
	class IndexBuffer
	{
		using Index = GLuint;
	public:
		IndexBuffer(const std::vector<Index>& data, GLBufferUsage usage);
		IndexBuffer() = default;
		IndexBuffer(IndexBuffer&& other) noexcept;
		~IndexBuffer();

		IndexBuffer& operator=(IndexBuffer&& other) noexcept;

		void Bind() const;
		void Unbind() const;

		inline Index GetCount() const { return m_Count; }
	private:
		GLuint m_RendererID;
		GLuint m_Count;
	};
}
