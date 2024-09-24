#include "glash/glash_pch.hpp"

#include "glash/Window.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	glash::Window window(SCR_WIDTH, SCR_HEIGHT, "Example 2");

	//Initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	ImGui_ImplGlfw_InitForOpenGL(window.GetPtr(), true);
	ImGui_ImplOpenGL3_Init(window.GetGLVersion().c_str());

	bool show_demo_window = true;
	bool show_another_window = false;
	float slider_value = 0.5f;
	int button_clicks = 0;
	
	while (!window.ShouldClose())
	{
        window.ClearBuffer();

		window.PollEvents();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        // Create a window
        {
            ImGui::Begin("Hello, world!");
            ImGui::Text("This is some text.");
            ImGui::SliderFloat("Slider", &slider_value, 0.0f, 1.0f);
            if (ImGui::Button("Button")) {
                button_clicks++;
            }
            ImGui::Text("Button clicked %d times", button_clicks);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.SwapBuffers();
	}
}