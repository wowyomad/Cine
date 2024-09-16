#define GLM_ENABLE_EXPERIMENTAL
#include "glash/glash_pch.hpp"

#include "glash/window.hpp"
#include "glash/Renderer.hpp"
#include "glash/shader.hpp"
#include "glash/mesh_triangle.hpp"
#include "glash/mesh_rectangle.hpp"
#include "glash/Shader.hpp"
#include "glash/helper/file_reader.hpp"

#include "glm/gtx/string_cast.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


inline void normalize(std::vector<glm::vec3>& positions, int width, int height)
{
	for (glm::vec3& pos : positions)
	{
		pos.x = 2.0f * pos.x / width - 1.0f;
		pos.y = 2.0f * pos.y / height - 1.0f;
	}
}
inline void RunTestWindow()
{
	try {
		glash::Window window(800, 800, "Test Window");
		glash::Renderer renderer;

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

		std::vector <glm::vec3> rect_positions =
		{
		   glm::vec3(300, 300.0f, .0f),
		   glm::vec3(500, 300.0f, .0f),
		   glm::vec3(500.0f, 500.0f, .0f),
			glm::vec3(300.0f, 500.0f, .0f)
		};

		normalize(rect_positions, w, h);

		std::vector <glm::vec3> rect_colors = 
		{
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.5f, 0.0f, 0.5f),
			glm::vec3(0.5, 0.5, 1.0f)
		};

		auto shader1 = glash::Shader("resources/shaders/shader.shader");
		
		auto rect = glash::mesh::RectangleMesh({0.0f, 0.0f, 0.0f}, { 0.8, 0.0, 0.9 }, 0.25, 0.45);

		std::vector<glm::vec3> tri_positoins =
		{
			{250.0f, 250.0f, 0.0f},
			{450.0f, 250.0f, 0.0f},
			{350.0, 450.0f, 0.0f}
		};

		normalize(tri_positoins, w, h);

		std::vector<glm::vec3> tri_colors =
		{
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		};

		auto tri = glash::mesh::TriangleMesh(tri_positoins.data(), tri_colors.data());


		if (shader1)
		{
			window.SetClearColor(glash::color::GREEN);
		}
		else
		{
			window.SetClearColor(glash::color::RED);
		}

		glm::vec3 offset = { 0.0f, 0.0f, 0.0f };
		glm::vec3 move = { 0.001f, 0.0f, 0.0f };

		size_t frames = 0;
		double startTime = glfwGetTime();


		const std::vector<float> vertices = {
			0.5 * -0.5f, 0.5 * -0.5f, 1.0f, 0.0f, 0.0f,
			0.5 * 0.5f, 0.5 * -0.5f, 0.25f, 0.0f, 0.0f,
			0.5 * 0.5f, 0.5 * 0.5f, 0.5f, 0.0f, 0.0f,
			0.5 * -0.5f, 0.5 * 0.5f, 0.75f, 0.0f, 0.0f
		};

		

		const std::vector<GLuint> indices = {
			0, 1, 2,
			0, 2, 3
		};

		glash::IndexBuffer ib(indices, glash::GLBufferUsage::STATIC_DRAW);
		glash::VertexBuffer vb(vertices, glash::GLBufferUsage::STATIC_DRAW);
		glash::VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(3);
		glash::VertexArray va;
		va.AddBuffer(vb, layout);

	


		while (!window.ShouldClose()) {

			window.PollEvents();
			window.ClearBuffer();

			shader1.Bind();
			shader1.SetUniform("brightness", 1.0f);
			rect.Draw();

			renderer.Draw(va, ib, shader1);

			window.SwapBuffers();
		}

	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	std::cerr << "Closing..." << std::endl;
}
