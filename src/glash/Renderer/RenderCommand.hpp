#pragma once

#include "glash/Core/Core.hpp"

#include "glash/Renderer/RendererAPI.hpp"

namespace glash
{
	class RenderCommand
	{
	public:
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}