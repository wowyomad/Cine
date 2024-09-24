#include "TestTexture.hpp"

namespace glash
{
	namespace test
	{
		TestTexture::TestTexture(Window& window) :
			Test(window),
			m_Shader(Shader("resources/shaders/shader_with_texture.shader")),
			m_Texture(Texture("resources/textures/face.png"))
		{
			float modelWidth = 512, modelHeight = 512;
			float modelScale = 1.0f;

			int w, h;
			window.GetWindowSize(&w, &h);
			m_Projection = glm::ortho<float>(0, w, 0, h);
			m_View = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });
			m_Model = glm::translate(glm::mat4(1.0f), { w / 2, h / 2, 0.0 });

			if (m_Shader)
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

			m_IndexBuffer = IndexBuffer(indices, glash::GLBufferUsage::DYNAMIC_DRAW);
			m_VertexBuffer = VertexBuffer(vertices, glash::GLBufferUsage::DYNAMIC_DRAW);
			VertexBufferLayout layout;
			layout.Push<float>(2);
			layout.Push<float>(2);

			m_VertexArray.AddBuffer(m_VertexBuffer, layout);

		}
		void TestTexture::OnUpdate(float deltaTime)
		{
			//TODO: move model
			int w, h;
			m_Window.GetWindowSize(&w, &h);
			m_Projection = glm::ortho<float>(0, w, 0, h);
			m_Model = glm::translate(glm::mat4(1.0f), {w / 2, h / 2, 0.0});
		}
		void TestTexture::OnImGuiRender()
		{

		}
		void TestTexture::OnRender()
		{
			m_Texture.Bind(0);
			m_Shader.Bind();
			m_Shader.SetSamplerSlot("u_Texture", glash::GLSampler::SAMPLER_2D, 0);

			glm::mat4 MVP = m_Projection * m_View * m_Model;
			m_Shader.SetUniform("uMVP", MVP);
			m_Renderer.Draw(m_VertexArray, m_IndexBuffer, m_Shader);
		}
	}
}
