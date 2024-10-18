#include "glash/glash_pch.hpp"
#include "Sandbox2D.hpp"

#include "Scripts/ColorScript.hpp"
#include "Scripts/ControllerScript.hpp"

void Sandbox2D::OnAttach()
{
	m_Scene = Cine::CreateRef<Cine::Scene>();

	RegisterComponents();

	m_Player = m_Scene->CreateEntity("Player");
	m_Player.AddComponent<Cine::SpriteRendererComponent>();

	auto texture = m_TextureLibrary.LoadTexture2D("Face.png", "Assets/Textures/Face.png");
	 m_Player.AddComponent<Cine::SpriteSheetComponent>(texture);

	auto& transform = m_Player.GetComponent<Cine::TransformComponent>();
	transform.Scale = { 5.0f, 5.0f, 1.0f };

	m_Player.AddComponent<ColorScript>();
	m_Player.GetComponent<Cine::TransformComponent>().Translation.z = 0.25f;
	auto& controller = m_Player.AddComponent<ControllerScript>();
	controller.Speed = 5.0f;

	auto player2 = m_Scene->CreateEntity("Player 2");
	player2.AddComponent<Cine::SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
	player2.AddComponent<ControllerScript>().Speed = -5.0f;

	auto camera = m_Scene->CreateEntity("Camera");
	auto& cameraComponent = camera.AddComponent<Cine::CameraComponent>();
	m_Scene->SetMainCamera(camera);
	cameraComponent.Camera.SetProjectionType(Cine::SceneCamera::ProjectionType::Perspective);
	camera.GetComponent<Cine::TransformComponent>().Translation.z = 10.0;

	m_Scene->OnViewportResize(1280, 720);
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Cine::Timestep ts)
{
	m_Scene->OnUpdateRuntime(ts);
}

void Sandbox2D::OnEvent(Cine::Event& event)
{
	Cine::EventDispatcher d(event);
	d.Dispatch<Cine::WindowResizeEvent>(CINE_BIND_EVENT_FN(Sandbox2D::OnWindowResize));
}


bool Sandbox2D::OnWindowResize(Cine::WindowResizeEvent& e)
{
	m_Scene->OnViewportResize(e.GetWidth(), e.GetHeight());
	return false;
}

void Sandbox2D::RegisterComponents()
{
	m_Scene->RegisterComponent<ControllerScript>();
	m_Scene->RegisterComponent<ColorScript>();
}

