#include "glash/glash_pch.hpp"
#include "glash/Renderer/VertexArray.hpp"

#include "glash/Renderer/Renderer.hpp"

namespace Cine
{
	Ref<VertexArray> VertexArray::Create()
	{
		CINE_PROFILE_FUNCTION();

		return Renderer::Get().CreateVertexArray();
	}
}