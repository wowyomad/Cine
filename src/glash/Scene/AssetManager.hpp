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

		SpriteSheetComponent LoadSpriteSheet(std::string name, const std::filesystem::path& path);

	private:
		TextureLibrary m_TextureLibrary;
	};
}