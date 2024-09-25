#pragma once

#include "glash/Core.hpp"
#include "Test.hpp"
#include "glash/Shader.hpp"
#include "glash/IndexBuffer.hpp"
#include "glash/VertexArray.hpp"
#include "glash/VertexBuffer.hpp"
#include "glash/Texture.hpp"
#include "glash/Renderer.hpp"
#include "glm/matrix.hpp"

namespace glash
{
	namespace test
	{
		class TestTexture : public Test
		{
		public: 
			TestTexture(Window& window);

			void OnUpdate(float deltaTime) override;
			void OnImGuiRender() override;
			void OnRender() override;


		private:
			Renderer m_Renderer;
			Shader m_Shader;
			VertexArray m_VertexArray;
			VertexBuffer m_VertexBuffer;
			IndexBuffer m_IndexBuffer;
			Texture m_Texture;
			
			glm::mat4 m_Model, m_View, m_Projection;
		};
	}
}