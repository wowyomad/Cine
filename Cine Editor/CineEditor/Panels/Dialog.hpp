#pragma once

#include <imgui.h>
#include <functional>

namespace Cine
{
	struct EditorNotification
	{
		bool Show = false;
		std::string Message = "";
		ImVec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float Duration = 600.0f;
		float Elapsed = 0.0f;

		EditorNotification() = default;
		EditorNotification(const std::string& message, const ImVec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float duration = 600.0f)
			: Message(message), Color(color), Duration(duration), Show(true) { };
	};

	inline bool ShowConfirmationPopup(const char* title, const char* message, std::function<void()> onConfirm)
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
	inline void ShowNotification(const std::string& message, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) // Default color is white
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 windowSize = io.DisplaySize;
		ImVec2 viewportPos = ImGui::GetMainViewport()->Pos;

		// Calculate the height of the notification based on the text size
		const float minHeight = 100.0f;
		const float padding = 10.0f;
		ImVec2 textSize = ImGui::CalcTextSize(message.c_str());
		float notificationHeight = std::max(textSize.y + 2 * padding, minHeight);

		ImVec2 notificationSize = ImVec2(300, notificationHeight);
		ImVec2 position = ImVec2(viewportPos.x + (windowSize.x - notificationSize.x) / 2, viewportPos.y + windowSize.y / 3 - notificationSize.y / 2); // Center on x and one-third on y

		ImGui::SetNextWindowPos(position, ImGuiCond_Always);
		ImGui::SetNextWindowSize(notificationSize); // Set window size
		ImGui::Begin("Notification", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);

		ImGui::PushStyleColor(ImGuiCol_Text, color); // Set text color

		// Center the text horizontally and vertically
		ImVec2 textPos = ImVec2((notificationSize.x - textSize.x) / 2, (notificationSize.y - textSize.y) / 2);
		ImGui::SetCursorPos(textPos);
		ImGui::Text("%s", message.c_str());

		ImGui::PopStyleColor();

		ImGui::End();
	}

	inline void ShowNotification(const std::string& message, bool* showNotification, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) // Default color is white
	{
		if (!showNotification || !*showNotification)
			return;

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 windowSize = io.DisplaySize;
		ImVec2 viewportPos = ImGui::GetMainViewport()->Pos;

		// Calculate the height of the notification based on the text size
		const float minHeight = 100.0f;
		const float padding = 10.0f;
		ImVec2 textSize = ImGui::CalcTextSize(message.c_str());
		float notificationHeight = std::max(textSize.y + 2 * padding, minHeight);

		ImVec2 notificationSize = ImVec2(300, notificationHeight);
		ImVec2 position = ImVec2(viewportPos.x + (windowSize.x - notificationSize.x) / 2, viewportPos.y + windowSize.y / 3 - notificationSize.y / 2); // Center on x and one-third on y

		ImGui::SetNextWindowPos(position, ImGuiCond_Always);
		ImGui::SetNextWindowSize(notificationSize); // Set window size
		ImGui::Begin("Notification", showNotification, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

		ImGui::PushStyleColor(ImGuiCol_Text, color); // Set text color

		// Center the text horizontally and vertically
		ImVec2 textPos = ImVec2((notificationSize.x - textSize.x) / 2, (notificationSize.y - textSize.y) / 2);
		ImGui::SetCursorPos(textPos);
		ImGui::Text("%s", message.c_str());

		ImGui::PopStyleColor();

		ImGui::End();
	}







}