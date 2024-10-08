#include "Sandbox2D.hpp"

void Sandbox2D::OnAttach()
{
	m_SquareTexture = Cine::Texture2D::Create("resources/textures/face.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Cine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Cine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Cine::Renderer2D::DrawQuad(m_SquarePosition + glm::vec3(1.5f, 0.5f, 0.0f), m_SquareSize * 1.5f, m_SquareColor);
	Cine::Renderer2D::DrawQuad(m_SquarePosition, m_SquareSize, m_SquareTexture, m_SquareColor);
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
	ImGui::DragFloat2("Square position", glm::value_ptr(m_SquarePosition), 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat2("Square size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 10.0f);
	ImGui::End();
}
