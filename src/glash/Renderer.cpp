#include "glash/Renderer.hpp"

namespace glash
{

	void Renderer_OLD::Draw(const VertexArray_OLD& va, const IndexBuffer_OLD& ib, const Shader_OLD& m_Shader) const
	{
		va.Bind();
		ib.Bind();
		m_Shader.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, (void*)0));

		//va.Unbind();
		//ib.Unbind();
		//shader.Unbind();
	}
}
