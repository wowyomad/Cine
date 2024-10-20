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

	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != m_AssetsDirectory)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (auto& p : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path =  p.path();
			auto relativePath = std::filesystem::relative(path, m_AssetsDirectory);
			std::string filenameString = relativePath.filename().string();

			if (p.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{

				}
			}
			
		}

		ImGui::End();
	}

}
