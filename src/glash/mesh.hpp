#include "glash_pch.hpp"

namespace glash
{
	namespace mesh
	{
		class TriangleMesh
		{
			static const size_t COLUMNS = 6;
			static const size_t ROWS = 3;
		public:
			inline TriangleMesh(const float positions[3][3], const float colors[3][3])
			{
				float data[6 * 3];

				for (size_t i = 0; i < ROWS; i++)
				{
					memcpy(data + i * COLUMNS, positions + i, 3 * sizeof(float));
					memcpy(data + 3 + i * COLUMNS, colors + i, 3 * sizeof(float));
				}

				glGenVertexArrays(1, &m_Vao);
				glBindVertexArray(m_Vao);
				

				glGenBuffers(1, &m_Vbo);
				glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
				glEnableVertexAttribArray(0);

				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
				glEnableVertexAttribArray(1);

			}

			inline void draw()
			{
				glBindVertexArray(m_Vao);
				glDrawArrays(GL_TRIANGLES, 0, m_szVertexCount);
			}

			inline ~TriangleMesh()
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
			const GLsizei m_szVertexCount = 3;
		};

	}
}