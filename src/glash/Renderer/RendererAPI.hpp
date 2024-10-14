#pragma once

#include <functional>

#include "glash/Core/Base.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/VertexArray.hpp"
#include "glash/Renderer/FrameBuffer.hpp"

namespace Cine
{
	class CINE_RENDERER_API_CLASS;
	class IRendererAPI;

	using RendererAPI = CINE_RENDERER_API_CLASS;

	class IRendererAPI
	{
	public:
		virtual ~IRendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0; 

		virtual void SetLineWidth(float width) = 0;

		virtual Ref<VertexBuffer> CreateVertexBuffer(const float*, uint32_t size) = 0;
		virtual Ref<VertexBuffer> CreateVertexBufferEmpty(uint32_t size) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(const uint32_t*, uint32_t size) = 0;
		virtual Ref<VertexArray> CreateVertexArray() = 0;
		virtual Ref<FrameBuffer> CreateFrameBuffer(const FramebufferSpecification& specification) = 0;

		static Ref<RendererAPI> Create();

		static inline Ref<RendererAPI> Get() { return s_Instance;  }

	private:
		static Ref<RendererAPI> s_Instance;

	};
}
// this is stupid but it works (x2)
#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"
