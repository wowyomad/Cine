#pragma once

#include <functional>

#include "glash/Core/Core.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/VertexArray.hpp"

namespace glash
{
	class GLASH_RENDERER_API_CLASS;

	class RendererAPI
	{
	public:


		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0; 

		static Scope<GLASH_RENDERER_API_CLASS> Create();
		static Ref<VertexBuffer> CreateVertexBuffer(const float*, uint32_t size);
		static Ref<VertexBuffer> CreateVertexBufferEmpty(uint32_t size);
		static Ref<IndexBuffer> CreateIndexBuffer(const uint32_t*, uint32_t size);
		static Ref<VertexArray> CreateVertexArray();


		static inline  GLASH_RENDERER_API_CLASS& GetAPI() { return *s_Instance;  }

	private:
		static Ref<GLASH_RENDERER_API_CLASS> s_Instance;


	};
}