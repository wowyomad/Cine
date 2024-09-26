#pragma once

#include "glash/Window_OLD.hpp"
#include "imgui.h"

class ImguiWindow
{
public:
	ImguiWindow(const glash::Window_OLD& window);
	void NewImguiFrame();
	void ImguiRender();

	ImGuiContext* GetImGuiContext() const;
};
