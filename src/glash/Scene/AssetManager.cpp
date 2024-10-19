#include "glash/glash_pch.hpp"
#include "AssetManager.hpp"

#include "SceneSerializer.hpp"

namespace Cine
{
    AssetManager* AssetManager::s_Instance = nullptr;

    void AssetManager::Init()
    {
        s_Instance = new AssetManager();
    }

    SpriteSheetComponent AssetManager::LoadSpriteSheet(std::string name, const std::filesystem::path& relativePath, bool defaultFrame)
    {
        std::filesystem::path path = "Assets" / relativePath;

        Ref<Texture2D> texture = s_Instance->m_TextureLibrary.LoadTexture2D(name, path);

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
        else if(defaultFrame)
        {
            SpriteSheetComponent::Frame defaultFrame;
            defaultFrame.Coords[0] = { 0.0f, 0.0f };
            defaultFrame.Coords[1] = { 1.0f, 0.0f };
            defaultFrame.Coords[2] = { 1.0f, 1.0f };
            defaultFrame.Coords[3] = { 0.0f, 1.0f };

            spriteSheet.Frames.push_back(defaultFrame);
        }

        return spriteSheet;
    }
}

