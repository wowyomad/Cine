#include "glash/Cine.hpp"

class Sandbox2D : public Cine::Layer
{
public:
	Sandbox2D(Cine::Application* ptr) : Cine::Layer("Sandbox")
	{

	};

	void OnAttach() override final;
	void OnDetach() override final;

	void OnUpdate(Cine::Timestep ts) override final;
	void OnEvent(Cine::Event& event) override final;

	void RegisterComponents();

private:

	bool OnWindowResize(Cine::WindowResizeEvent& e);
private:

	Cine::Ref<Cine::Scene> m_Scene;
	Cine::TextureLibrary m_TextureLibrary;
	struct WindowData
	{
		uint32_t width = 0, height = 0;
	};
	WindowData m_WindowSize = {};
	WindowData m_ResizeWindowSize = {};

	Cine::Entity m_Player;
};