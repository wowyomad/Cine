#pragma once

#include "glash/Scene/AssetManager.hpp"
#include "glash/Scene/Scene.hpp"

#include "glash/Renderer/Texture.hpp"

namespace Cine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel(const std::filesystem::path& directory = AssetManager::AssetsDirectory);
		void OnImGuiRender();

		void SetContext(Ref<Scene> scene);
		bool DisplayReloadScriptsButton();
	
	private:
		std::filesystem::path m_AssetsDirectory;
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture> m_DirectoryIcon;
		Ref<Texture> m_FileIcon;

		float m_UpdateInterval = 1.0f;

		//Should be project though
		Ref<Scene> m_ActiveScene;
	};
}