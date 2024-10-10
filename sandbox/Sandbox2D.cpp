#include "Sandbox2D.hpp"

#include "glash/Core/Timer.hpp"
#include "glash/Debug/Instrumentor.hpp"

void Sandbox2D::OnAttach()
{
	m_CheckerBoardTexture = Cine::Texture2D::Create("resources/textures/checkerboard.png");
	m_FaceTexture = Cine::Texture2D::Create("resources/textures/face.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Cine::Timestep ts)
{
	CINE_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	Cine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Cine::Renderer2D::DrawQuad({ 0.5f, 0.5f, 0.0f }, { 2.0f, 1.5f }, { 0.6f, 0.9f, 0.4f, 1.0f });
	Cine::Renderer2D::DrawQuad({ -1.5f, -0.5f, 0.0f }, { 1.0f, 1.5f }, { 0.9f, 0.4f, 0.5f, 1.0f });
	//Cine::Renderer2D::DrawRotatedQuad(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(1.0, 1.0f) * 500.0f, 45.0f, m_CheckerBoardTexture, 500.0f, {0.8f, 1.0f, 0.8f, 1.0f});
	//Cine::Renderer2D::DrawRotatedQuad(m_SquarePosition, m_SquareSize, m_SquareRotation, m_FaceTexture);
	Cine::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Cine::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	CINE_PROFILE_FUNCTION();

	ImGui::Begin("Sandbox");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::DragFloat2("Square position", glm::value_ptr(m_SquarePosition), 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat2("Square size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 10.0f);
	ImGui::DragFloat("Square rotation", &m_SquareRotation, 1.0f, std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
	if (ImGui::Checkbox("VSync", &m_VSync))
	{
		s_Application->GetWindow().SetVSync(m_VSync);
	}
	if (ImGui::Button("Reset"))
	{
		m_CameraController.Reset();
		m_SquarePosition = glm::vec3(0.0f, 0.0f, 0.0f);
		m_SquareRotation = 0.0f;
		m_SquareSize = glm::vec2(1.0f, 1.0f);
	}

	ImGui::End();
}
