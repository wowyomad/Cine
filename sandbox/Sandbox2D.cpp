#include "Sandbox2D.hpp"

void Sandbox2D::OnAttach()
{
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

	m_SquareVertexArray = Cine::VertexArray::Create();

	auto SquareVertexBuffer = Cine::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));
	auto SquareIndexBuffer = Cine::IndexBuffer::Create(indicesSquare, sizeof(indicesSquare));

	Cine::BufferLayout SquareLayout = {
		{Cine::ShaderDataType::Float3, "a_Position"}
	};
	SquareVertexBuffer->SetLayout(SquareLayout);

	m_SquareVertexArray->AddVertexBuffer(SquareVertexBuffer);
	m_SquareVertexArray->SetIndexBuffer(SquareIndexBuffer);

	m_SquareShader = Cine::Shader::Create("resources/shaders/Renderer2D_Quad.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Cine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Cine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Cine::Renderer2D::DrawQuad(m_SquarePosition, m_SquareSize, m_SquareColor);
	Cine::Renderer2D::EndScene();

}

void Sandbox2D::OnEvent(Cine::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Sandbox");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::DragFloat2("Square position", glm::value_ptr(m_SquarePosition), 0.1f, -10.0f, 10.0f, "%.2f");
	ImGui::DragFloat2("Square size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 10.0f, "%.2f");
	ImGui::End();
}
