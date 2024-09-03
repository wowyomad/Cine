#define GLM_ENABLE_EXPERIMENTAL

#include "glash/glash_pch.hpp"
#include "glash/window.hpp"
#include "glash/shader.hpp"
#include "glash/mesh_triangle.hpp"
#include "glash/mesh_rectangle.hpp"

#include "glm/gtx/string_cast.hpp"


inline void normalize(glm::vec3* positions, int width, int height)
{
	for (size_t i = 0; i < 4; i++)
	{
		positions[i].x = 2.0f * positions[i].x / width - 1.0f;
		positions[i].y = 2.0f * positions[i].y / height - 1.0f;
	}
}
inline void RunTestWindow()
{
	LOG_INFO("Fuck this {}", "shit");
	try {
		glash::Window window(800, 800, "Test Window");

		window.SetKeyCallback([](GLFWwindow* window, int key, int scanconde, int action, int mods) -> void
			{
				switch (action)
				{
				case GLFW_PRESS:
				{
					switch (key)
					{
					case GLFW_KEY_ESCAPE:
						glfwSetWindowShouldClose(window, true);
						break;
					default:
						break;
					}
					break;
				}
				default:
					break;
				}

			}
		);

		int w = 0, h = 0;
		window.GetWindowSize(&w, &h);

		std::vector <glm::vec3> positions = {
		   glm::vec3(200.0f, 200.0f, .0f),
		   glm::vec3(600.0f, 200.0f, .0f),
		   glm::vec3(600.0f, 400.0f, .0f),
		   glm::vec3(200.0f, 400.0f, .0f),
		};
		std::vector <glm::vec3> colors = {
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
		};
		normalize(positions.data(), w, h);
		auto mesh1 = glash::mesh::RectangleMesh(positions.data(), colors.data());

		positions[0] = positions[3];
		positions[1] = positions[2];
		positions[2] = (positions[0] + positions[1]) / 2.0f;
		positions[2].y += 0.25f;

		std::swap(colors[0], colors[1]);

		auto mesh2 = glash::mesh::TriangleMesh(positions.data(), colors.data());


		auto builder = glash::ShaderProgram::Builder();
		bool success =
			builder.AddShader("shaders/vertex.vert", glash::SHADER_TYPE::VERTEX_SHADER)
			&&
			builder.AddShader("shaders/fragment.frag", glash::SHADER_TYPE::FRAGMENT_SHADER);

		auto shader = builder.Build();
		if (shader)
		{
			window.SetClearColor(glash::color::GREEN);
		}
		else
		{
			window.SetClearColor(glash::color::RED);
		}

		while (!window.ShouldClose()) {
			window.PollEvents();
			window.ClearBuffer();

			shader.Use();
			mesh1.draw();
			mesh2.draw();
			shader.Reset();


			window.SwapBuffers();
		}

	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	std::cerr << "Closing..." << std::endl;
}
