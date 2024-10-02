#pragma once
#include "glash/Core/Core.hpp"
#if defined GLASH_PLATFORM_OPENGL && GLASH_PLATFORM_OPENGL == 1

#include "glash/Renderer/VertexArray.hpp"

namespace glash
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() override;
		void Unbind() override;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers;  }
		inline const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer;  }

	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}

#endif // GLASH_PLATFORM_OPENGL