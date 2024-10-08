#pragma once

#include "glash/Cine.hpp"
#include "Sandbox2D.hpp"

using namespace Cine;

static Application* s_Application = nullptr;

class SimpleLayer : public Layer
{
public:
	SimpleLayer()
		: Layer("Simple Sandbox Layer"),
		m_CameraController(1.7778, true)
	{
		float verticesSquare[] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};

		unsigned int indicesSquare[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_VertexArrayTriangle = VertexArray::Create();
		m_SquareVertexArray = VertexArray::Create();

		auto SquareVertexBuffer = VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));
		auto SquareIndexBuffer = IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(float));

		BufferLayout SquareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		SquareVertexBuffer->SetLayout(SquareLayout);

		m_SquareVertexArray->AddVertexBuffer(SquareVertexBuffer);
		m_SquareVertexArray->SetIndexBuffer(SquareIndexBuffer);

		auto specification = TextureSpecification::Default2D();
		specification.MagFilter = TextureFilter::Nearest;
		specification.Wrap = TextureWrap::Repeat;
		specification.GenerateMips = false;
		m_Texture = Texture2D::Create("resources/textures/checkerboard.png", specification);

		m_ShaderLibrary.Load("Simple", "resources/shaders/uniform_color.glsl");
		m_Shader = m_ShaderLibrary.Get("Simple");
	}
	void OnFixedUpdate(Timestep fixedDeltaTime)  override
	{

	}

	void OnUpdate(Timestep deltaTime) override
	{
		m_CameraController.OnUpdate(deltaTime);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_SquareScale));

		Renderer::BeginScene(m_CameraController.GetCamera());
		m_Shader->Bind();
		m_Shader->SetFloat4("u_Color", { 0.4f, 1.0f, 0.3f, 1.0f });
		m_Texture->Bind(0);

		for (size_t i = 0; i < m_SquareRows; i++)
		{
			for (size_t j = 0; j < m_SquareColumns; j++)
			{
				glm::mat4 tranform = glm::translate(glm::mat4(1.0f), m_SquarePosition + glm::vec3(i * m_SquareScale, j * m_SquareScale, 0.0f));
				glm::mat4 squareTransform = tranform * scale;
				if ((i + j) % 2 == 0)
				{
					m_Shader->SetFloat4("u_Color", m_SquareColor1);
				}
				else
				{
					m_Shader->SetFloat4("u_Color", m_SquareColor2);
				}
				Renderer::Submit(m_Shader, m_SquareVertexArray, squareTransform);
			}
		}
		Renderer::EndScene();

	}

	void OnEvent(Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::SliderInt("Rows", &m_SquareRows, 1, 128);
		ImGui::SliderInt("Columns", &m_SquareColumns, 1, 128);
		ImGui::DragFloat("Square Scale", &m_SquareScale, 0.05f, 0.0f, 10.0f, "%.2f");
		ImGui::ColorEdit4("Square 1", glm::value_ptr(m_SquareColor1));
		ImGui::ColorEdit4("Square 2", glm::value_ptr(m_SquareColor2));

		ImGui::SliderFloat("Camera Translation Speed", &m_CameraController.CameraTranslatoinSpeed, 0.1f, 10.0f);
		ImGui::SliderFloat("Camera Rotation Speed", &m_CameraController.CameraRotationSpeed, 1.0f, 360.0f);

		if (ImGui::Checkbox("VSync", &m_VSync))
		{
			s_Application->GetWindow().SetVSync(m_VSync);
		}

		ImGui::End();
	}

	bool OnMouseScrolledEvent(MouseScrolledEvent& event)
	{
		return false;
	}

	bool OnWindowResizeEvent(WindowResizeEvent& event)
	{

		return false;
	}

private:
	Ref<VertexArray> m_SquareVertexArray;

	glm::vec3 m_SquarePosition = glm::vec3(0.0f);
	glm::vec3 m_TargetSquarePosition = m_SquarePosition;

	float m_SquareMoveLerpFactor = 0.01f;
	float m_SquareSpeed = 0.5f;
	float m_SquareMoveWaitTime = 0.0f;
	float m_SquareMoveCooldwon = 0.0f;
	float m_SquareScale = 1.0f;
	int m_SquareRows = 1;
	int m_SquareColumns = 1;
	glm::vec4 m_SquareColor1 = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 m_SquareColor2 = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	float m_CameraScale = 1.0f;
	float m_CameraMinScale = 0.1f;
	float m_TargetCameraScale = m_CameraScale;
	float m_CameraScaleSpeed = 0.1f;
	float m_CameraScaleLerpFactor = 0.01f;
	bool m_VSync = false;

	Ref<VertexArray> m_VertexArrayTriangle;
	Ref<Shader> m_Shader;
	ShaderLibrary m_ShaderLibrary;
	Ref<Texture> m_Texture;
	OrthograhpicCameraController m_CameraController;
};

class SandboxApp : public Application
{
public:
	SandboxApp()
	{
		m_Window->SetVSync(false);
		s_Application = this;
		//PushOverlay(new SimpleLayer());
		PushOverlay(new Sandbox2D());
	}

	~SandboxApp()
	{

	}
};

