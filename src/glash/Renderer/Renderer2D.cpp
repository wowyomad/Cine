#include "glash/glash_pch.hpp"
#include "glash/Renderer/Renderer2D.hpp"
#include "glash/Renderer/RenderCommand.hpp"

#include "glash/Renderer/VertexArray.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/Shader.hpp"

namespace Cine
{
	uint32_t Renderer2D::QuadCount = 0;
	uint32_t Renderer2D::DrawCalls = 0;

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 100000;
		static const uint32_t MaxVertices = 4 * MaxQuads;
		static const uint32_t MaxIndices = 6 * MaxQuads;


		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;
		
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase;
		QuadVertex* QuadVertexBufferPtr;

	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord"},
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
			
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6, offset += 4)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		s_Data.QuadShader = Shader::Create("resources/shaders/Renderer2D_Quad.glsl");
		s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xFFFFFFFF;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_Data.QuadShader->Bind();



	}
	void Renderer2D::Shutdown()
	{
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{ 
		RenderCommand::Clear();


		s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		DrawCalls = 0;
		QuadCount = 0;
	}
	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		Renderer2D::Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
	
		if (s_Data.QuadIndexCount == s_Data.MaxIndices)
		{
			DrawCalls += 1;
			EndScene();
			s_Data.QuadIndexCount = 0;
			s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
			DrawQuad(position, size, color);
		}

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0 };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0 };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0 };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0 };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		QuadCount += 1;

		return;

		//glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(size.x, size.y, 1.0f));
		//DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(size.x, size.y, 1.0f));
		DrawQuad(transform, texture, tiling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)), glm::vec3(size.x, size.y, 1.0f));
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)), glm::vec3(size.x, size.y, 1.0f));
		DrawQuad(transform, texture, tiling, tintColor);
	}
	
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(size.x, size.y, 1.0f));
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(size.x, size.y, 1.0f));
		DrawQuad(transform, texture, tiling, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 1.0f)), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(size.x, size.y, 1.0f));
		DrawQuad(transform, color);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 1.0f)), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(size.x, size.y, 1.0f));
		DrawQuad(transform, texture, tiling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		CINE_PROFILE_FUNCTION();


		s_Data.QuadShader->SetMat4("u_Transform", transform);
		s_Data.QuadShader->SetFloat4("u_Color", color);
		s_Data.QuadShader->SetInt("u_Texture", 0);
		s_Data.QuadShader->SetFloat("u_Tiling", 1.0f);

		s_Data.WhiteTexture->Bind(0);
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		CINE_PROFILE_FUNCTION();

		s_Data.QuadShader->SetMat4("u_Transform", transform);
		s_Data.QuadShader->SetFloat4("u_Color", tintColor);
		s_Data.QuadShader->SetInt("u_Texture", 0);
		s_Data.QuadShader->SetFloat("u_Tiling", tiling);

		texture->Bind(0);
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}


}