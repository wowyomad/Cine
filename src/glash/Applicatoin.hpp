#pragma once
#include "glash_pch.hpp"
#include "glash/Core.hpp"

#include "glash/events/ApplicationEvent.hpp"
#include "glash/Window.hpp"
#include "glash/LayerStack.hpp"

namespace glash
{
	class GLASH_API Application
	{
	public:
		Application();
		virtual ~Application() { };

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get();
		Window& GetWindow();

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);

	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running;

		static Application* s_Instance;
	};
	//Should be defined in Client
	Application* CreateApplication();
}