#pragma once
#include "glash/Core.hpp"
#include "glash_pch.hpp"

#include "glash/events/ApplicationEvent.hpp"
#include "glash/LayerStack.hpp"
#include "glash/ImGui/ImGuiLayer.hpp"
#include "glash/Input.hpp"
#include "glash/Window.hpp"

namespace glash
{
	class GLASH_WINDOW_CLASS;

	class GLASH_API Application
	{
	public:
		/**
		* This shit has to be inlined so that the constructor definition exists on client side.
		* When calling Application(0), it calls private constructor defined on library side and 
		* sets the context for ImGui. And when back to this consructor, the ImGuiContext 
		* pointer is passed to the client. 
		* Otherwise, the client would have NULL in ImGui context. Too bad!
		*/
		Application()
			: Application(0)
		{
			ImGuiContext* p_context = GetImGuiContext();

			if (p_context != nullptr)
			{
				ImGui::SetCurrentContext(p_context);
			}
		}
		virtual ~Application() {  };

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		ImGuiContext* GetImGuiContext() const;
		void GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data) const;


		static Application& Get();
		GLASH_WINDOW_CLASS& GetWindow();

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);

		Application(int dummy);

	private:
		std::unique_ptr<GLASH_WINDOW_CLASS> m_Window;
		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		bool m_Running;

		static Application* s_Instance;
	};
	//Should be defined in Client
	Application* CreateApplication();
}