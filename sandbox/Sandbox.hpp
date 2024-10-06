#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "glash/Glash.hpp"
#include "glash/Core/Application.hpp"
#include "glash/Core/Input.hpp"

#include "glash/Renderer/VertexArray.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/Shader.hpp"
#include "glash/Renderer/Renderer.hpp"

#include "glash/events/Event.hpp"

#include <GLFW/glfw3.h>

using namespace glash;

static Application* s_Application = nullptr;

class SimpleLayer : public glash::Layer
{
public:
	SimpleLayer()
		: Layer("Simple Sandbox Layer"),
		m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		//float verticesSquare[] = {
		//	-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.3f, 1.0,
		//	 0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.3f, 1.0,
		//	 0.5f,  0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0,
		//	-0.5f,  0.5f, 0.0f,		0.2f, 0.8f, 0.3f, 1.0
		//};

		float verticesSquare[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,	
		};

		unsigned int indicesSquare[] = {
			0, 1, 2,
			2, 3, 0
		};


		m_VertexArrayTriangle = VertexArray::Create();
		m_VertexArraySquare = VertexArray::Create();

		auto SquareVertexBuffer = VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));
		auto SquareIndexBuffer = IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(float));

		BufferLayout SquareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
		};
		SquareVertexBuffer->SetLayout(SquareLayout);

		m_VertexArraySquare->AddVertexBuffer(SquareVertexBuffer);
		m_VertexArraySquare->SetIndexBuffer(SquareIndexBuffer);

		m_Shader = Shader::Create("resources/shaders/uniform_color.shader");
	}
	void OnFixedUpdate(Timestep fixedDeltaTime)  override
	{

	}

	void OnUpdate(Timestep deltaTime) override
	{
		m_SquarePosition.x = std::lerp(m_SquarePosition.x, m_TargetSquarePosition.x,  1 - powf(m_SquareMoveLerpFactor, deltaTime));
		m_SquarePosition.y = std::lerp(m_SquarePosition.y, m_TargetSquarePosition.y, 1 - powf(m_SquareMoveLerpFactor, deltaTime));
		m_CameraScale = std::lerp(m_CameraScale, m_TargetCameraScale, 1 - powf(m_CameraScaleLerpFactor, deltaTime));


		glm::vec3 squareTranslation(0.0f);
		if (Input::IsKeyDown(Key::A))
		{
			squareTranslation.x -= 1;
		}
		if (Input::IsKeyDown(Key::D))
		{
			squareTranslation.x += 1;
		}
		if (Input::IsKeyDown(Key::W))
		{
			squareTranslation.y += 1;
		}
		if (Input::IsKeyDown(Key::S))
		{
			squareTranslation.y -= 1;
		}

		if (m_TargetCameraScale < m_CameraMinScale)
		{
			m_TargetCameraScale = m_CameraMinScale;
		}

		float ratio = (float)s_Application->GetWindow().GetWidth() / (float)s_Application->GetWindow().GetHeight();
		float halfRation = ratio / 2;
		m_Camera.SetProjection(m_CameraScale * -halfRation, m_CameraScale * halfRation, m_CameraScale * -0.5, m_CameraScale * 0.5f);

		if (m_SquareMoveCooldwon <= 0.0f && glm::dot(squareTranslation, squareTranslation) > 0.0)
		{
			m_TargetSquarePosition += m_SquareSpeed * glm::normalize(squareTranslation);
			m_SquareMoveCooldwon = m_SquareMoveWaitTime;
		}
		else
		{
			m_SquareMoveCooldwon -= deltaTime;
		}

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));


		Renderer::BeginScene(m_Camera);
		m_Shader->Bind();
		{
			for (size_t i = 0; i < m_SquareRows; i++)
			{
				for (size_t j = 0; j < m_SquareColumns; j++)
				{
					glm::mat4 tranform = glm::translate(glm::mat4(1.0f), m_SquarePosition + glm::vec3(i * m_SquareOffset, j * m_SquareOffset, 0.0f));
					glm::mat4 squareTransform = tranform * scale;
					if ((i + j) % 2 == 0)
					{
						m_Shader->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
					}
					else
					{
						m_Shader->SetFloat4("u_Color", { 0.0f, 0.0f, 0.0f, 1.0f });
					}
					Renderer::Submit(m_Shader, m_VertexArraySquare, squareTransform);
				}
			}
		}
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
		ImGui::SliderFloat("Scale", &m_TargetCameraScale, 0.1f, 10.0f, "%.1f");
		ImGui::SliderFloat("Square Speed", &m_SquareSpeed, 0.1f, 10.0f, "%.1f");
		ImGui::SliderInt("Rows", &m_SquareRows, 1, 128);
		ImGui::SliderInt("Columns", &m_SquareColumns, 1, 128);
		ImGui::DragFloat("Offset", &m_SquareOffset, 0.05f, 0.0f, 10.0f, "%.2f");

		ImGui::DragFloat("Square Move Lerp Factor", &m_SquareMoveLerpFactor, 0.01, 0.01f, 1.0f, "%.2f");
		ImGui::DragFloat("Camera Scale Lerp Factor", &m_CameraScaleLerpFactor, 0.01, 0.01f, 1.0f, "%.2f");


		if (ImGui::Checkbox("VSync", &m_VSync))
		{
			s_Application->GetWindow().SetVSync(m_VSync);
		}
		
		ImGui::End();
	}

	bool OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		m_TargetCameraScale -= m_CameraScaleSpeed * event.GetVertical();
		return true;

	}

	bool OnWindowResizeEvent(WindowResizeEvent& event)
	{
		float ratio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(m_CameraScale * -ratio, m_CameraScale * ratio, m_CameraScale * -1.0, m_CameraScale * 1.0f);
		return false;
	}

private:
	Ref<VertexArray> m_VertexArraySquare;

	glm::vec3 m_SquarePosition = glm::vec3(0.0f);
	glm::vec3 m_TargetSquarePosition = m_SquarePosition;
	float m_SquareMoveLerpFactor = 0.1f;
	float m_SquareSpeed = 0.5f;
	float m_SquareMoveWaitTime = 0.0f;
	float m_SquareMoveCooldwon = 0.0f;
	float m_SquareOffset = 0.25f;
	int m_SquareRows = 1;
	int m_SquareColumns = 1;

	bool m_VSync = false;


	float m_CameraScale = 1.0f;
	float m_CameraMinScale = 0.1f;
	float m_TargetCameraScale = m_CameraScale;
	float m_CameraScaleSpeed = 0.1f;
	float m_CameraScaleLerpFactor = 0.1f;

	Ref<VertexArray> m_VertexArrayTriangle;
	Ref<Shader> m_Shader;
	OrthographicCamera m_Camera;

};

class Sandbox : public glash::Application
{
public:
	Sandbox()
	{
		m_Window->SetVSync(false);
		s_Application = this;
		PushOverlay(new SimpleLayer());
	}

	~Sandbox()
	{

	}
};

