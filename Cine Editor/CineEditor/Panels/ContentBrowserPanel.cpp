#include "glash/glash_pch.hpp"
#include "ContentBrowserPanel.hpp"

#include "glash/Scene/AssetManager.hpp"

#include "glash/Core/Timer.hpp"

#include <filesystem>

#include <imgui.h>	

namespace Cine
{
	ContentBrowserPanel::ContentBrowserPanel(const std::filesystem::path& directory)
		: m_AssetsDirectory(directory), m_CurrentDirectory(m_AssetsDirectory)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != m_AssetsDirectory)
		{
			if (ImGui::Button("<"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		float padding = 8.0f;
		float thumbnailSize = 96;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = static_cast<int>(panelWidth / cellSize);
		if (columnCount <= 0)
		{
			columnCount = 1;
		}

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, m_AssetsDirectory);
			std::string filenameString = relativePath.filename().string();


			ImTextureID icon = directoryEntry.is_directory() ? (ImTextureID)m_DirectoryIcon->GetRendererID() : (ImTextureID)m_FileIcon->GetRendererID();
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
			ImGui::ImageButton(filenameString.c_str(), icon, { thumbnailSize , thumbnailSize }, { 0, 1 }, { 1, 0 });


			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			ImGui::TextWrapped(filenameString.c_str());



			ImGui::NextColumn();

		}

		ImGui::End();
	}

}
