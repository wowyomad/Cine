#include "glash/glash_pch.hpp"
#include "glash/logger.hpp"
#include "glash/structures.hpp"
#include "glash/VertexBuffer.hpp"
#include "glash/IndexBuffer.hpp"
#include "glash/VertexArray.hpp"	
#include "glash/VertexBufferLayout.hpp"

namespace glash
{
	namespace mesh
	{
		class RectangleMesh
		{
			static const size_t ROWS = 4;

		public:
			inline RectangleMesh(const glm::vec3& position, const glm::vec3& color, const float width, const float height)
			{
				glm::vec3 positions[4] = {
					{position.x - width, position.y - height, 0.0f},
					{position.x + width, position.y - height, 0.0f},
					{position.x + width, position.y + height, 0.0f},
					{position.x - width, position.y + height, 0.0f},
				};

				glm::vec3 colors[4] =
				{
					color, color, color, color
				};
				
				*this = RectangleMesh(positions, colors);
			}

			inline RectangleMesh(const glm::vec3 positions[4], const glm::vec3 colors[4], enum GLBufferUsage bufferUsage = GLBufferUsage::STATIC_DRAW)
			{
				std::vector<glm::vec3> vertices(4 + 4);

				for (size_t i = 0; i < ROWS; i++)
				{
					vertices[i * 2] = positions[i];
					vertices[1 + i * 2] = colors[i];
				}

				std::vector<IndexBuffer::Index> indices{
					0, 1, 2 ,
					0, 2, 3
				};

				va = VertexArray();
				va.Bind();
				vb = VertexBuffer(vertices, GLBufferUsage::STATIC_DRAW);
				ib = IndexBuffer(indices, GLBufferUsage::STATIC_DRAW);
				VertexBufferLayout layout;
				layout.Push<float>(3);
				layout.Push<float>(3);
				va.AddBuffer(vb, layout);

				va.Unbind();
				ib.Unbind();
				vb.Unbind();
			}

			inline void Draw()
			{
				va.Bind();
				GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, (void*)0));
				va.Unbind();
			}


		private:
			VertexBuffer vb;
			IndexBuffer ib;
			VertexArray va;
		};
	}
}
