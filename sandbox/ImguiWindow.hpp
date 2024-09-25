#include "glash/Window.hpp"
#include "imgui.h"

class ImguiWindow
{
	
public:
	ImguiWindow(const glash::Window& window);
	void NewImguiFrame();
	void ImguiRender();

	ImGuiContext* GetImGuiContext() const;

};
