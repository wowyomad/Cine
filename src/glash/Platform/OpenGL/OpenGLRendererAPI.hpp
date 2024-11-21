#pragma once
#include "glash/Core/Base.hpp"
#if CINE_PLATFORM_OPENGL

#include "glash/Renderer/RendererAPI.hpp"

namespace Cine
{
	class OpenGLRendererAPI : public IRendererAPI
	{
	public:
		OpenGLRendererAPI()
		{
			std::cout << "OpenGLRendererAPI\n";
		}

		void Init() override final;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override final;

		void SetClearColor(const glm::vec4& color) override final;
		void Clear() override final;

		void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override final;
		void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override final;
		void SetLineWidth(float width) override final;

		Ref<VertexBuffer> CreateVertexBuffer(const float*, uint32_t size) override final;
		Ref<VertexBuffer> CreateVertexBufferEmpty(uint32_t size) override final;
		Ref<IndexBuffer> CreateIndexBuffer(const uint32_t*, uint32_t size) override final;
		Ref<VertexArray> CreateVertexArray() override final;
		Ref<FrameBuffer> CreateFrameBuffer(const FramebufferSpecification& specification) override final;

		void SetDepthTest(bool enabled) override final;
		bool GetDepthTest() const override final;

	private:
		bool m_DepthTestEnabled = false;
	};
}

#endif // GLASH_API_OPENGL