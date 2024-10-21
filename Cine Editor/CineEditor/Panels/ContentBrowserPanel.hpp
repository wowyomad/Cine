#pragma once

#include "glash/Scene/AssetManager.hpp"

#include "glash/Renderer/Texture.hpp"

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

		Ref<Texture> m_DirectoryIcon;
		Ref<Texture> m_FileIcon;

		float m_UpdateInterval = 1.0f;
	};
}