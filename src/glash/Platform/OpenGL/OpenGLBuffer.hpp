#pragma once
#include "glash/Core/Core.hpp"
#if GLASH_PLATFORM_OPENGL

#include "glash/Renderer/Buffer.hpp"

namespace glash
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void SetData(const void* data, uint32_t size) override;

		void SetLayout(const BufferLayout& layout) override;
		const BufferLayout& GetLayout() const override;

		void Bind() const override;
		void Unbind() const override;

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t size);

		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		uint32_t GetCount() const override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}

#endif // GLASH_PLATFORM_OPENGL