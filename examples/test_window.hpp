#define GLM_ENABLE_EXPERIMENTAL
#include "glash/glash_pch.hpp"

#include "glash/window.hpp"
#include "glash/Renderer.hpp"
#include "glash/shader.hpp"
#include "glash/mesh_triangle.hpp"
#include "glash/mesh_rectangle.hpp"
#include "glash/Shader.hpp"
#include "glash/helper/file_reader.hpp"
#include "glash/Texture.hpp"
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
glash::Shader* g_Shader = nullptr;

inline void RunTestWindow()
{
	try {
		glash::Window window(800, 800, "Test Window");
		glash::Renderer renderer;

		auto shader = glash::Shader("resources/shaders/shader_with_texture.shader");
		auto texture = glash::Texture("resources/textures/face.png");
		texture.Bind();
		if (shader)
		{
			window.SetClearColor(glash::color::GREEN);
		}
		else
		{
			window.SetClearColor(glash::color::RED);
		}

		const std::vector<float> vertices = {
			-0.2f, -0.2f,	0.0f, 0.0f,
			 0.2f, -0.2f,	1.0f, 0.0f,
			 0.2f,  0.2f,	1.0f, 1.0f,
			-0.2f,  0.2f,	0.0f, 1.0f
		};

		const std::vector<GLuint> indices = {
			0, 1, 2,
			0, 2, 3
		};

		glash::IndexBuffer ib(indices, glash::GLBufferUsage::STATIC_DRAW);
		glash::VertexBuffer vb(vertices, glash::GLBufferUsage::STATIC_DRAW);
		glash::VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		glash::VertexArray va;
		va.AddBuffer(vb, layout);

		g_Shader = &shader;

		window.SetKeyCallback([](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
				g_Shader->Reload();
				std::cout << "Shader reloaded!" << std::endl;
			}
			});


		while (!window.ShouldClose()) {

			window.PollEvents();
			window.ClearBuffer();

			shader.Bind();
			shader.SetSamplerSlot("u_Texture", glash::GLSampler::SAMPLER_2D, 0);
			renderer.Draw(va, ib, shader);

			window.SwapBuffers();
		}

	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	std::cerr << "Closing..." << std::endl;
}
