#pragma once

#include "glash/Core/Core.hpp"
#include "glash/Enums.hpp"
#include "glash/glash_pch.hpp"
#include "glash/Core/Log.hpp"

namespace glash
{
	class GLASH_API VertexBuffer
	{

	public:
		template<class T>
		 VertexBuffer(const std::vector<T>& data, GLBufferUsage usage)
			: VertexBuffer()
		{
			GLCall(glGenBuffers(1, &m_RendererID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage));
		}

		VertexBuffer() : m_RendererID(0) {}
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer(const VertexBuffer& other) = default;
		~VertexBuffer();

		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(const VertexBuffer& other) = default;

		void Bind() const;
		static void Unbind();

	protected:
		GLuint m_RendererID;
	};

	
}
