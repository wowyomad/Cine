#pragma once
#include "glash/Core/Core.hpp"
#if GLASH_PLATFORM_OPENGL

#include "glash/Renderer/RendererAPI.hpp"

namespace glash
{
	class OpenGLRendererAPI : public IRendererAPI
	{
	public:
		OpenGLRendererAPI() = default;

		virtual void Init() override final;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override final;

		virtual void SetClearColor(const glm::vec4& color) override final;
		virtual void Clear() override final;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override final;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override final;
		virtual void SetLineWidth(float width) override final;

		virtual Ref<VertexBuffer> CreateVertexBuffer(const float*, uint32_t size) override final;
		virtual Ref<VertexBuffer> CreateVertexBufferEmpty(uint32_t size) override final;
		virtual Ref<IndexBuffer> CreateIndexBuffer(const uint32_t*, uint32_t size) override final;
		virtual Ref<VertexArray> CreateVertexArray() override final;

	};
}

#endif // GLASH_API_OPENGL