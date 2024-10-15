#include "ImGuiLayer.hpp"
#include "glash/glash_pch.hpp"

#include "glash/Core/Application.hpp"
#include "glash/Core/Log.hpp"

#include "GLFW/glfw3.h" //temp

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"




namespace Cine
{
	ImGuiLayer::ImGuiLayer(const std::string& name)
		: Layer(name) {}

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") {}

	ImGuiLayer::~ImGuiLayer() { }

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


		float fontSize = 16.0f;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/JetBrainsMono/ttf/JetBrainsMono-Regular.ttf", fontSize);
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/JetBrainsMono/ttf/JetBrainsMono-Bold.ttf", fontSize);

		//ImGui::StyleColorsDark();
		//SetDarkTheme();

		ImGui::StyleColorsLight();
		SetLightTheme();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void Cine::ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::SetBlockEvents(bool blockEvents)
	{
		m_BlockEvents = blockEvents;
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkTheme()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

	void ImGuiLayer::SetLightTheme()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.95f, 0.96f, 0.98f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.86f, 0.87f, 0.89f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.78f, 0.79f, 0.81f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.76f, 0.77f, 0.79f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.80f, 0.81f, 0.84f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.72f, 0.73f, 0.76f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.64f, 0.65f, 0.68f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.90f, 0.91f, 0.93f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.82f, 0.83f, 0.85f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.76f, 0.77f, 0.79f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.80f, 0.81f, 0.83f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.86f, 0.87f, 0.89f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.78f, 0.79f, 0.81f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.80f, 0.81f, 0.83f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.86f, 0.87f, 0.89f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.92f, 0.93f, 0.95f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.88f, 0.89f, 0.91f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f, 0.96f, 0.98f, 1.0f };
	}
}