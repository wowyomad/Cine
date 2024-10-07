#include "glash/glash_pch.hpp"
#include "glash/Renderer/VertexArray.hpp"

#include "glash/Renderer/Renderer.hpp"

namespace Cine
{
	Ref<VertexArray> VertexArray::Create()
	{
		return Renderer::Get().CreateVertexArray();
	}
}