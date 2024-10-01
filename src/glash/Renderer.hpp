#pragma once
#include "glash/Core/Core.hpp"
#include "glash/VertexArray.hpp"
#include "glash/IndexBuffer.hpp"
#include "glash/Shader.hpp"

namespace glash
{
	class GLASH_API Renderer_OLD
	{
	public:
		void Draw(const VertexArray_OLD& va, const IndexBuffer_OLD& ib, const Shader_OLD& m_Shader) const;
	};
}
