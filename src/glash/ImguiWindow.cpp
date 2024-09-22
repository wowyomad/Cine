#include "glash/glash_pch.hpp"
#include "ImguiWindow.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace glash
{
	ImguiWindow::ImguiWindow(int width, int height, const std::string& title, const Color& clearColor)
		: Window(width, height, title, clearColor)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(GetPtr(), true);
		ImGui_ImplOpenGL3_Init(GetGLVersion().c_str());
	}
	void ImguiWindow::NewImguiFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImguiWindow::ImguiRender()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}