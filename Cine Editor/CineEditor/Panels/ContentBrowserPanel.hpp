#pragma once

#include "glash/Scene/AssetManager.hpp"

namespace Cine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel(const std::filesystem::path& directory = AssetManager::AssetsDirectory);
		void OnImGuiRender();
	
	private:
		std::filesystem::path m_AssetsDirectory;
		std::filesystem::path m_CurrentDirectory;

		float m_UpdateInterval = 1.0f;
	};
}