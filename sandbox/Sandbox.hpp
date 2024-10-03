#pragma once

#include "glash/Glash.hpp"
#include "glash/Core/Application.hpp"
#include "glash/Core/Input.hpp"

#include "glash/Renderer/VertexArray.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/Shader.hpp"
#include "glash/Renderer/Renderer.hpp"

#include "glash/events/Event.hpp"

using namespace glash;

static Application* s_Application = nullptr;

class SimpleLayer : public glash::Layer
{
public:
	SimpleLayer()
		: Layer("Simple Sandbox Layer"),
		m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{ 
		float verticesSquare[] = {
			-0.75f, -0.75f, 0.0f,		0.8f, 0.2f, 0.3f, 1.0,
			 0.75f, -0.75f, 0.0f,		0.2f, 0.8f, 0.3f, 1.0,
			 0.75f,  0.75f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0,
			-0.75f,  0.75f, 0.0f,		0.2f, 0.8f, 0.3f, 1.0
		};

		float verticesTriangle[] = {
			-0.5f * cosf(3.1415 / 6.0f),		-0.5f * sinf(3.1415 / 6.0f),		0.0f,		1.0f, 1.0f, 1.0f, 0.25f,
			 0.5f * cosf(3.1415 / 6.0f),		-0.5f * sinf(3.1415 / 6.0f),	0.0f,		1.0f, 1.0f, 1.0f, 0.25f,
			 0.0f,								 0.5f,								0.0f,		1.0f, 1.0f, 1.0f, 0.25f,
		};
		unsigned int indicesSquare[] = {
			0, 1, 2,
			2, 3, 0
		};
		unsigned int indicesTriangle[] = {
			0, 1, 2,
		};

		m_VertexArrayTriangle = VertexArray::Create();
		m_VertexArraySquare = VertexArray::Create();

		auto SquareVertexBuffer = VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));
		auto SquareIndexBuffer = IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(float));

		auto TriangleVertexBuffer = VertexBuffer::Create(verticesTriangle, sizeof(verticesTriangle));
		auto TriangleIndexBuffer = IndexBuffer::Create(indicesTriangle, sizeof(indicesTriangle) / sizeof(float));

		BufferLayout SquareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		SquareVertexBuffer->SetLayout(SquareLayout);

		BufferLayout TriangleLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		SquareVertexBuffer->SetLayout(SquareLayout);
		TriangleVertexBuffer->SetLayout(TriangleLayout);

		m_VertexArraySquare->AddVertexBuffer(SquareVertexBuffer);
		m_VertexArraySquare->SetIndexBuffer(SquareIndexBuffer);
		m_VertexArrayTriangle->AddVertexBuffer(TriangleVertexBuffer);
		m_VertexArrayTriangle->SetIndexBuffer(TriangleIndexBuffer);

		m_Shader = Shader::Create("resources/shaders/shader.shader");
	}


	void OnUpdate() override
	{
		float ratio = (float)s_Application->GetWindow().GetWidth() / (float)s_Application->GetWindow().GetHeight();
		m_Camera.SetProjection(m_CameraScale * -ratio, m_CameraScale * ratio, m_CameraScale * -1.0, m_CameraScale * 1.0f);

		auto cameraPositoin = m_Camera.GetPosition();
		float speed = 0.01;
		glm::vec3 translation(0.0f);

		if (Input::IsKeyPressed(Key::A))
		{
			translation.x -= 1;
		}
		if (Input::IsKeyPressed(Key::D))
		{
			translation.x += 1;
		}
		if (Input::IsKeyPressed(Key::W))
		{
			translation.y += 1;
		}
		if (Input::IsKeyPressed(Key::S))
		{
			translation.y -= 1;
		}
		m_TriangleRotation += 0.5f;

		if (glm::dot(translation, translation) > 0.0)
		{
			translation = glm::normalize(translation) * speed;
		}
		m_Camera.SetPosition(cameraPositoin + -translation);

		glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_TriangleRotation), glm::vec3(0.0f, 0.0f, 1.0f));

		Renderer::BeginScene(m_Camera);
			m_Shader->Bind();
			m_Shader->SetInt("u_UseUniformColor", 0);
			Renderer::Submit(m_Shader, m_VertexArraySquare);
			m_Shader->SetInt("u_UseUniformColor", 1);
			m_Shader->SetFloat4("u_Color", m_TriangleColor);
			Renderer::Submit(m_Shader, m_VertexArrayTriangle, transform);
		Renderer::EndScene();
	}

	void OnEvent(glash::Event& event) override
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseScrolledEvent>(GLASH_BIND_EVENT_FN(SimpleLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(GLASH_BIND_EVENT_FN(SimpleLayer::OnWindowResizeEvent));

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::SliderFloat4("Color", glm::value_ptr(m_TriangleColor), 0.0f, 1.0f, "%.2f");
		ImGui::End();
	}

	bool OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		m_CameraScale -= 0.05f * event.GetVertical();
		return true;
	}

	bool OnWindowResizeEvent(WindowResizeEvent& event)
	{
		float ratio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(m_CameraScale * -ratio, m_CameraScale * ratio, m_CameraScale * -1.0, m_CameraScale * 1.0f);
		return false;
	}

private:
	glm::vec4 m_TriangleColor = glm::vec4(1.0f);
	float m_TriangleRotation = 0.0f;
	float m_CameraScale = 1.0f;
	
	Ref<VertexArray> m_VertexArrayTriangle;
	Ref<VertexArray> m_VertexArraySquare;
	Ref<Shader> m_Shader;
	OrthographicCamera m_Camera;

};

class Sandbox : public glash::Application
{
public:
	Sandbox()
	{
		s_Application = this;
		PushOverlay(new SimpleLayer());
	}

	~Sandbox()
	{
		
	}
};

