 #include "glash/glash_pch.hpp"
#include "glash/logger.hpp"

namespace glash
{
	namespace mesh
	{
		class RectangleMesh
		{
			static const size_t COLUMNS = 6;
			static const size_t ROWS = 4;

		public:
			inline RectangleMesh(const glm::vec3 positions[4], const glm::vec3 colors[4])
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

				//2 triangles to make up a rectangle
				GLuint indices[3 * 2] = {	
					0, 1, 2,
					0, 2, 3
				};

				glGenVertexArrays(1, &m_Vao);
				glBindVertexArray(m_Vao);

				glGenBuffers(1, &m_Vbo);
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
				GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

				glGenBuffers(1, &m_Ebo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
				GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

				GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, COLUMNS * sizeof(float), (void*)0));
				glEnableVertexAttribArray(0);

				GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, COLUMNS * sizeof(float), (void*)(3 * sizeof(float))));
				glEnableVertexAttribArray(1);

				glBindVertexArray(0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

			inline void Draw()
			{
				GLCall(glBindVertexArray(m_Vao));
				GLCall(glDrawElements(GL_TRIANGLES, m_szVertexCount, GL_UNSIGNED_INT, (void*)0));
			}

			inline ~RectangleMesh()
			{
				if (m_Vao != 0)
				{
					glDeleteVertexArrays(1, &m_Vao);
				}
				if (m_Vbo != 0)
				{
					glDeleteBuffers(1, &m_Vbo);
				}
				if (m_Ebo != 0)
				{
					glDeleteBuffers(1, &m_Ebo);
				}
				
			}

		private:
			GLuint m_Vao = 0;
			GLuint m_Vbo = 0;
			GLuint m_Ebo = 0;
			const GLsizei m_szVertexCount = 6;
		};
	}
}
