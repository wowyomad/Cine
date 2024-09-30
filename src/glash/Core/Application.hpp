#pragma once
#include "glash/glash_pch.hpp"
#include "glash/Core/Core.hpp"

#include "glash/Core/LayerStack.hpp"
#include "glash/Core/Window.hpp"
#include "glash/Core/Input.hpp"

#include "glash/ImGui/ImGuiLayer.hpp"

#include "glash/events/ApplicationEvent.hpp"

namespace glash
{
	class GLASH_WINDOW_CLASS;

	class Application
	{
	public:
		/**
		* This shit has to be inlined so that the constructor definition exists on client side.
		* When calling Application(0), it calls private constructor defined on library side and 
		* sets the context for ImGui. And when back to this consructor, the ImGuiContext 
		* pointer is passed to the client. 
		* Otherwise, the client would have NULL in ImGui context. That's a feature🥴
		*/
		Application()
			: Application(0)
		{
			GLASH_LOG_TRACE(BUILD_STR);
			ImGuiContext* p_context = GetImGuiContext();
			ImGuiContext* this_context = ImGui::GetCurrentContext();

			if (p_context != nullptr)
			{
				ImGui::SetCurrentContext(p_context);
			}
		}
		GLASH_API virtual ~Application() {  };

		GLASH_API void Run();
		GLASH_API void OnEvent(Event& event);

		GLASH_API void PushLayer(Layer* layer);
		GLASH_API void PushOverlay(Layer* overlay);

		GLASH_API ImGuiContext* GetImGuiContext() const;
		GLASH_API void GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data) const;


		GLASH_API static Application& Get();
		GLASH_API GLASH_WINDOW_CLASS& GetWindow();

	private:
		GLASH_API bool OnWindowCloseEvent(WindowCloseEvent& event);
		GLASH_API bool OnWindowResizeEvent(WindowResizeEvent& event);

		GLASH_API Application(int dummy);

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