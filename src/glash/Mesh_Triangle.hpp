#include "glash/glash_pch.hpp"
#include "glash/Core.hpp"

#include "glash/VertexBuffer.hpp"
#include "glash/VertexArray.hpp"
#include "glash/Log.hpp"

namespace glash
{
	namespace mesh
	{
		class GLASH_API TriangleMesh
		{
			static const size_t ROWS = 3;
		public:
			inline TriangleMesh(const glm::vec3 positions[3], const glm::vec3 colors[3])
			{
				std::vector<glm::vec3> vertices(ROWS * 2);
				
				for (size_t i = 0; i < ROWS; i++)
				{ 
					vertices[i * 2] = positions[i];
					vertices[1 + i * 2] = colors[i];
				}
				vb = VertexBuffer(vertices, GLBufferUsage::STATIC_DRAW);
				VertexBufferLayout layout;
				layout.Push<float>(3);
				layout.Push<float>(3);

				va = VertexArray();
				va.AddBuffer(vb, layout);

				vb.Unbind();
				va.Unbind();

			}

			inline void Draw()
			{
				va.Bind();
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
			}
		private:
			VertexArray va;
			VertexBuffer vb;
		};

	}
}
