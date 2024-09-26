#pragma once
#include "glash/Core.hpp"
#include "glash/VertexArray.hpp"
#include "glash/IndexBuffer.hpp"
#include "glash/Shader.hpp"

namespace glash
{
	class GLASH_API Renderer
	{
	public:
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	};
}
