#include "glash/glash_pch.hpp"
#include "glash/Renderer/Renderer2D.hpp"
#include "glash/Renderer/RenderCommand.hpp"

#include "glash/Renderer/VertexArray.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/Shader.hpp"

namespace Cine
{
	struct RendererData
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;

		glm::mat4 ViewProjection;
	};

	static RendererData s_Data;

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		float quadVertices[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};
		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices));
		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		quadVertexBuffer->SetLayout({
			{ ShaderDataType::Float4, "a_Position" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(quadVertexBuffer);
		s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
		s_Data.QuadShader = Shader::Create("resources/shaders/Renderer2D_Quad.glsl");
	}
	void Renderer2D::Shutdown()
	{
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{ 
		RenderCommand::Clear();
		s_Data.ViewProjection = camera.GetViewProjectionMatrix();
	}
	void Renderer2D::EndScene()
	{
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform =glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_Transform", transform);
		s_Data.QuadShader->SetMat4("u_ViewProjection", s_Data.ViewProjection);
		s_Data.QuadShader->SetFloat4("u_Color", color);

		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}
}