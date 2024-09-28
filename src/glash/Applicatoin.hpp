#pragma once
#include "glash/Core.hpp"
#include "glash_pch.hpp"

#include "glash/events/ApplicationEvent.hpp"
#include "glash/LayerStack.hpp"

namespace glash
{
	class GLASH_WINDOW_CLASS;

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
		GLASH_WINDOW_CLASS& GetWindow();

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);

	private:
		std::unique_ptr<GLASH_WINDOW_CLASS> m_Window;
		LayerStack m_LayerStack;
		bool m_Running;

		static Application* s_Instance;
	};
	//Should be defined in Client
	Application* CreateApplication();
}