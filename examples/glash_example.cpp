#include "glash/glash_pch.hpp"

#include "glash/Window.hpp"
#include "glash/Renderer.hpp"
#include "glash/Shader.hpp"
#include "glash/Texture.hpp"


glash::Shader* g_Shader = nullptr;

int main(int argc, char** argv)
{
	try {
		glash::Window window(800, 800, "Test Window");
		glash::Renderer renderer;

		auto shader = glash::Shader("resources/shaders/shader_with_texture.shader");
		auto texture = glash::Texture("resources/textures/face.png");
		float modelWidth = 512, modelHeight = 512;
		float modelScale = 0.5;
		if (shader)
		{
			window.SetClearColor(glash::color::GREEN);
		}
		else
		{
			window.SetClearColor(glash::color::RED);
		}

		const std::vector<float> vertices = {
			modelWidth / 2,	-modelHeight / 2,	0.0f, 0.0f,
			-modelWidth / 2, -modelHeight / 2,	1.0f, 0.0f,
			-modelWidth / 2, modelHeight / 2,	1.0f, 1.0f,
			modelWidth / 2, modelHeight / 2,	0.0f, 1.0f
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
			if (action == GLFW_PRESS)
			{
				if (key == GLFW_KEY_F5) {
					g_Shader->Reload();
					std::cout << "Shader reloaded!" << std::endl;
				}
			}
			});


		while (!window.ShouldClose()) {
			window.PollEvents();

			int w, h;
			window.GetWindowSize(&w, &h);

			window.ClearBuffer();

			shader.Bind();
			shader.SetSamplerSlot("u_Texture", glash::GLSampler::SAMPLER_2D, 0);

			auto projection = glm::ortho<float>(0, w, 0, h);
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, { w / 2, h / 2, 0 });
			model = glm::scale(model, { modelScale, modelScale, 1.0 });

			glm::mat4 MVP = projection * model;
			shader.SetUniform("uMVP", MVP);

			renderer.Draw(va, ib, shader);

			window.SwapBuffers();
		}

	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	std::cerr << "Closing..." << std::endl;
}