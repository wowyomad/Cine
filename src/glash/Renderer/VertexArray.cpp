#include "glash/glash_pch.hpp"
#include "glash/Renderer/VertexArray.hpp"

#include "glash/Renderer/Renderer.hpp"

namespace glash
{
	Ref<VertexArray> VertexArray::Create()
	{
		return RendererAPI::Get().CreateVertexArray();
	}
}