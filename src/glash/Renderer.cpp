#include "glash/Renderer.hpp"

namespace glash
{

	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
	{
		va.Bind();
		ib.Bind();
		shader.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, (void*)0));

		//va.Unbind();
		//ib.Unbind();
		//shader.Unbind();
	}
}
