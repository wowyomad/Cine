#include "glash/glash_pch.hpp"
#include "Sandbox2D.hpp"

#include "Scripts/ColorScript.hpp"

void Sandbox2D::OnAttach()
{
	m_Scene = Cine::CreateRef<Cine::Scene>();

	m_Player = m_Scene->CreateEntity("Player");
	m_Player.AddComponent<Cine::SpriteRendererComponent>();

	m_Scene->RegisterComponent<ColorScript>();
	m_Player.AddComponentByName("ColorScript");

	auto texture = m_TextureLibrary.LoadTexture2D("Face.png", "Assets/Textures/Face.png");
	auto& sheet = m_Player.AddComponent<Cine::SpriteSheetComponent>(texture);

	auto camera = m_Scene->CreateEntity("Camera");
	auto& cameraComponent = camera.AddComponent<Cine::CameraComponent>();
	m_Scene->SetMainCamera(camera);

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

void Sandbox2D::OnImGuiRender()
{
	
}

bool Sandbox2D::OnWindowResize(Cine::WindowResizeEvent& e)
{
	m_Scene->OnViewportResize(e.GetWidth(), e.GetHeight());
	return false;
}
