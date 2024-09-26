#include "glash/glash_pch.hpp"
#include "ImguiWindow.hpp"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

ImguiWindow::ImguiWindow(const glash::Window_OLD& window)
{
	glfwInit();
	glfwMakeContextCurrent(window.GetPtr());
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.GetPtr(), true);
	ImGui_ImplOpenGL3_Init(window.GetGLVersion().c_str());
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

ImGuiContext* ImguiWindow::GetImGuiContext() const
{
	return ImGui::GetCurrentContext();
}
ImGuiContext* GetImGuiContext(const ImguiWindow& window)
{
	return ImGui::GetCurrentContext();
}