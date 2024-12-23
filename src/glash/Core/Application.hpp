#pragma once
#define CINE_IMGUI 1
#include "glash/glash_pch.hpp"

#include "glash/Core/Base.hpp"
#include "glash/Core/LayerStack.hpp"
#include "glash/Core/Window.hpp"
#include "glash/Core/Input.hpp"

#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/Shader.hpp"
#include "glash/Renderer/VertexArray.hpp"
#include "glash/Renderer/Camera.hpp"

#include "glash/events/ApplicationEvent.hpp"
#include "glash/ImGui/ImGuiLayer.hpp"

#include "glash/Renderer/RendererAPI.hpp"

namespace Cine
{
	class CINE_WINDOW_CLASS;
	class IRendererAPI;

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
		Application(const std::string& title = "Cine Application")
			: Application(0)
		{
			ImGuiContext* p_context = GetImGuiContext();
			ImGuiContext* this_context = ImGui::GetCurrentContext();

			if (p_context != nullptr)
			{
				ImGui::SetCurrentContext(p_context);
			}
			m_Window->SetTitle(title);
		}
		GLASH_API virtual ~Application() {  };

		GLASH_API void Run();
		GLASH_API void OnEvent(Event& event);

		GLASH_API void PushLayer(Layer* layer);
		GLASH_API void PushOverlay(Layer* overlay);

		GLASH_API ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		GLASH_API ImGuiContext* GetImGuiContext() const;
		GLASH_API void GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data) const;
		GLASH_API void SetUpdateUI(bool update);


		GLASH_API static Application& Get();
		GLASH_API static void Set(Application* ptr);
		GLASH_API Window& GetWindow();
		GLASH_API void Close();

	private:
		GLASH_API bool OnWindowCloseEvent(WindowCloseEvent& event);
		GLASH_API bool OnWindowResizeEvent(WindowResizeEvent& event);

		GLASH_API Application(int dummy);

	protected:
		Ref<Window> m_Window;
		Ref<RendererAPI> m_Renderer;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running;
		bool m_UpdateUI = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;
		float m_TickTime = 1.0f / 60.0f;
		float m_TickAccumulator = 0.0f;

		static Application* s_Instance;
		
	};
	//Should be defined in Client
	Application* CreateApplication();
}