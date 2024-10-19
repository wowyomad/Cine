#pragma once

#include <unordered_map>
#include <filesystem>
#include "Components.hpp"

#include "glash/Renderer/Texture.hpp"

namespace Cine
{
	class AssetManager
	{
	public:

		static void Init();
		static SpriteSheetComponent LoadSpriteSheet(std::string name, const std::filesystem::path& path, bool defaultFrame = true);
	private:
		AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;

		TextureLibrary m_TextureLibrary;
		

		static AssetManager* s_Instance;
	};
}