#pragma once

#include "glash/Core/Core.hpp"
#include "glash/Enums.hpp"
#include "glash/glash_pch.hpp"
#include "glash/Core/Log.hpp"

namespace glash
{
	class GLASH_API VertexBuffer_OLD
	{

	public:
		template<class T>
		 VertexBuffer_OLD(const std::vector<T>& data, GLBufferUsage usage)
			: VertexBuffer_OLD()
		{
			GLCall(glGenBuffers(1, &m_RendererID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage));
		}

		VertexBuffer_OLD() : m_RendererID(0) {}
		VertexBuffer_OLD(VertexBuffer_OLD&& other) noexcept;
		VertexBuffer_OLD(const VertexBuffer_OLD& other) = default;
		~VertexBuffer_OLD();

		VertexBuffer_OLD& operator=(VertexBuffer_OLD&& other) noexcept;
		VertexBuffer_OLD& operator=(const VertexBuffer_OLD& other) = default;

		void Bind() const;
		static void Unbind();

	protected:
		GLuint m_RendererID;
	};

	
}
