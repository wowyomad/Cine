#include "glash/glash_pch.hpp"
#include "Sandbox2D.hpp"

void Sandbox2D::OnAttach()
{
	m_Scene = Cine::CreateRef<Cine::Scene>();

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
	
}

