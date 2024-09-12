#include "glash/glash_pch.hpp"
#include "glash/logger.hpp"
#include "glash/structures.hpp"
#include "glash/VertexBuffer.hpp"
#include "glash/IndexBuffer.hpp"

namespace glash
{
	namespace mesh
	{
		class RectangleMesh
		{
			static const size_t COLUMNS = 6;
			static const size_t ROWS = 4;

		public:
			inline RectangleMesh(const glm::vec3 positions[4], const glm::vec3 colors[4], enum GLBufferUsage bufferUsage = GLBufferUsage::STATIC_DRAW)
			{
				float vertices[COLUMNS * ROWS];

				const size_t positionOffset = 0;
				const size_t colorOffset = 3;

				memcpy(&vertices[positionOffset + 0 * COLUMNS], &positions[0], sizeof(glm::vec3));
				memcpy(&vertices[positionOffset + 1 * COLUMNS], &positions[1], sizeof(glm::vec3));
				memcpy(&vertices[positionOffset + 2 * COLUMNS], &positions[2], sizeof(glm::vec3));
				memcpy(&vertices[positionOffset + 3 * COLUMNS], &positions[3], sizeof(glm::vec3));
				memcpy(&vertices[colorOffset + 0 * COLUMNS], &colors[0], sizeof(glm::vec3));
				memcpy(&vertices[colorOffset + 1 * COLUMNS], &colors[1], sizeof(glm::vec3));
				memcpy(&vertices[colorOffset + 2 * COLUMNS], &colors[2], sizeof(glm::vec3));
				memcpy(&vertices[colorOffset + 3 * COLUMNS], &colors[3], sizeof(glm::vec3));

				GLuint indices[3 * 2] = {
					0, 1, 2,
					0, 2, 3
				};

				glGenVertexArrays(1, &m_Vao);
				glBindVertexArray(m_Vao);

				vb = VertexBuffer(std::vector<float>(vertices, vertices + ROWS * COLUMNS), GLBufferUsage::STATIC_DRAW);
				ib = IndexBuffer(std::vector(indices, indices + 3 * 2), GLBufferUsage::STATIC_DRAW);

				GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, COLUMNS * sizeof(float), (void*)0));
				glEnableVertexAttribArray(0);

				GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, COLUMNS * sizeof(float), (void*)(3 * sizeof(float))));
				glEnableVertexAttribArray(1);

				glBindVertexArray(0);
			}

			inline void Draw()
			{
				GLCall(glBindVertexArray(m_Vao));
				GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, (void*)0));
			}

			inline ~RectangleMesh()
			{
				if (m_Vao != 0)
				{
					glDeleteVertexArrays(1, &m_Vao);
				}

			}

			inline unsigned int GetVao()
			{
				return m_Vao;
			}


		private:
			VertexBuffer vb;
			IndexBuffer ib;
			GLuint m_Vao = 0;

			const GLsizei m_szVertexCount = 6;
		};
	}
}
