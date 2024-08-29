#include "glash/glash_pch.hpp"

namespace glash
{
	namespace mesh
	{
		class RectangleMesh
		{
			static const size_t COLUMNS = 6;
			static const size_t ROWS = 6;

		public:
			inline RectangleMesh(const glm::vec3 positions[4], const glm::vec3 colors[4])
			{
				float data[COLUMNS * ROWS];
				const size_t stride = 6;
				const size_t colorOffset = 3;
				const size_t positionOffset = 0;


				//Second triangle 
				memcpy(data + positionOffset + 0 * stride, positions + 0, sizeof(glm::vec3));
				memcpy(data + positionOffset + 1 * stride, positions + 1, sizeof(glm::vec3));
				memcpy(data + positionOffset + 2 * stride, positions + 2, sizeof(glm::vec3));

				memcpy(data + colorOffset + 0 * stride, colors + 0, sizeof(glm::vec3));
				memcpy(data + colorOffset + 1 * stride, colors + 1, sizeof(glm::vec3));
				memcpy(data + colorOffset + 2 * stride, colors + 2, sizeof(glm::vec3));

				//Second triangle
				memcpy(data + positionOffset + 3 * stride, positions + 0, sizeof(glm::vec3));
				memcpy(data + positionOffset + 4 * stride, positions + 2, sizeof(glm::vec3));
				memcpy(data + positionOffset + 5 * stride, positions + 3, sizeof(glm::vec3));

				memcpy(data + colorOffset + 3 * stride, colors + 0, sizeof(glm::vec3));
				memcpy(data + colorOffset + 4 * stride, colors + 2, sizeof(glm::vec3));
				memcpy(data + colorOffset + 5 * stride, colors + 3, sizeof(glm::vec3));

				glGenVertexArrays(1, &m_Vao);
				glBindVertexArray(m_Vao);

				glGenBuffers(1, &m_Vbo);
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
			}

			inline void draw()
			{
				glBindVertexArray(m_Vao);
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
				glDrawArrays(GL_TRIANGLES, 0, m_szVertexCount);
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
			}

		private:
			GLuint m_Vao = 0;
			GLuint m_Vbo = 0;
			const GLsizei m_szVertexCount = 6;
		};
	}
}
