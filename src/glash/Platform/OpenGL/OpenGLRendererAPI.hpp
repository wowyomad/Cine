#pragma once
#include "glash/Core/Core.hpp"
#if defined (GLASH_PLATFORM_OPENGL) == 1


#include "glash/Renderer/RendererAPI.hpp"


namespace glash
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override final;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override final;

		virtual void SetClearColor(const glm::vec4& color) override final;
		virtual void Clear() override final;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override final;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override final;

	};
}

#endif // GLASH_API_OPENGL