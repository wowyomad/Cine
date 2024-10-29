#pragma once

#include <imgui.h>
#include <functional>

namespace Cine
{
	bool ShowConfirmationPopup(const char* title, const char* message, std::function<void()> onConfirm)
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
	void ShowNotification(const std::string& message, float& timer, const float duration)
	{
		if (timer > 0.0f)
		{
			timer -= ImGui::GetIO().DeltaTime; 
		}

		ImVec2 windowSize = ImGui::GetIO().DisplaySize;
		ImVec2 notificationSize = ImVec2(300, 50);
		ImVec2 position = ImVec2(windowSize.x - notificationSize.x - 10, windowSize.y - notificationSize.y - 10);

		ImGui::SetNextWindowPos(position, ImGuiCond_Always);
		ImGui::Begin("Notification", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("%s", message.c_str());

		ImGui::Text("Time remaining: %.1f seconds", timer);

		ImGui::End();
	}
}