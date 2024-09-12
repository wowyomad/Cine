#pragma once

#include "glash/glash_pch.hpp"
#include "glash/logger.hpp"

namespace glash
{
	class VertexBuffer
	{
		
	public:
		template <class T>
		inline VertexBuffer(const std::vector<T>& data, GLBufferUsage usage)
		{
			GLCall(glGenBuffers(1, &m_RendererID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage));
		}
		VertexBuffer() = default;
		VertexBuffer(VertexBuffer&& other) noexcept;
		~VertexBuffer();

		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		void Bind() const;
		void Unbind() const;
	private:
		GLuint m_RendererID;
	};
}
