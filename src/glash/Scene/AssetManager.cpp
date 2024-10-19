#include "glash/glash_pch.hpp"
#include "AssetManager.hpp"

#include "SceneSerializer.hpp"

namespace Cine
{
    SpriteSheetComponent AssetManager::LoadSpriteSheet(std::string name, const std::filesystem::path& path)
    {
        Ref<Texture2D> texture = m_TextureLibrary.LoadTexture2D(name, path);

        SpriteSheetComponent spriteSheet;
        spriteSheet.Texture = texture;

        std::filesystem::path metaFilePath = path;
        metaFilePath.replace_extension(".shmeta");

        if (std::filesystem::exists(metaFilePath))
        {
            SpriteSheetMetaSerializer serializer(spriteSheet);
            if (!serializer.Deserialize(metaFilePath))
            {
                CINE_LOG_WARN("Meta file {} not found", metaFilePath.filename().string());
            }
        }
        else
        {
            SpriteSheetComponent::Frame defaultFrame;
            defaultFrame.Coords[0] = { 0.0f, 0.0f }; // Bottom-left
            defaultFrame.Coords[1] = { 1.0f, 0.0f }; // Bottom-right
            defaultFrame.Coords[2] = { 1.0f, 1.0f }; // Top-right
            defaultFrame.Coords[3] = { 0.0f, 1.0f }; // Top-left

            spriteSheet.Frames.push_back(defaultFrame);
        }

        return spriteSheet;
    }
}

