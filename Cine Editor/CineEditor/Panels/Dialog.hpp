#pragma once

#include <imgui.h>
#include <functional>

namespace Cine
{
	bool ShowConfirmationDialog(const char* title, const char* message, std::function<void()> onConfirm)
	{
		if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("%s", message);
			ImGui::Separator();

			if (ImGui::Button("Yes", { 120, 0 }))
			{
				onConfirm();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", { 120, 0 }))
			{
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return false;
			}

			ImGui::EndPopup();
		}
		return false;
	}
}