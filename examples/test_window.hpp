#define GLM_ENABLE_EXPERIMENTAL

#include "glash/glash_pch.hpp"
#include "glash/window.hpp"
#include "glash/shader.hpp"
#include "glash/mesh_triangle.hpp"
#include "glash/mesh_rectangle.hpp"

#include "glm/gtx/string_cast.hpp"
#include "glash/helper/file_reader.hpp"
#include <algorithm>

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
		   glm::vec3(300, 300.0f, .0f),
		   glm::vec3(500, 300.0f, .0f),
		   glm::vec3(500.0f, 500.0f, .0f),
			glm::vec3(300.0f, 500.0f, .0f)
		};

		normalize(positions, w, h);

		std::vector <glm::vec3> colors = {
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.5f, 0.0f, 0.5f),
			glm::vec3(0.5, 0.5, 1.0f)
		};

		auto builder = glash::ShaderProgram::Builder();

		builder.AddShaders(glash::ParseGLShaders("resources/shaders/shader.shader"));
		auto shader1 = builder.Build();
		
		auto rectangle_first = glash::mesh::RectangleMesh(positions.data(), colors.data(), glash::GLBufferUsage::DYNAMIC_DRAW);

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

		while (!window.ShouldClose()) {
			frames++;
			offset += move;

			if (offset.x >= 0.5f or offset.x <= -0.5f)
				move = -move;

			window.PollEvents();
			window.ClearBuffer();

			shader1.Use();
			shader1.SetUniformVec("time", { glfwGetTime(), 0.0f, 0.0f, 0.0f }, GL_FLOAT);
			shader1.SetUniformVec("offset", glm::vec4(offset, 0.0f), GL_FLOAT_VEC3);
			rectangle_first.Draw();

			window.SwapBuffers();
		}

		double elapsed = glfwGetTime() - startTime;
		fmt::println("Drawn {} frames in {} secoconds;\nFPS {}; Frame Time: {}", frames, elapsed, frames / elapsed, elapsed / frames);


	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	std::cerr << "Closing..." << std::endl;
}
