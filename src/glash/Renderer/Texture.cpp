#include "glash/glash_pch.hpp"
#include "glash/Renderer/Texture.hpp"

#include "glash/Platform/OpenGL/OpenGLTexture.hpp"

namespace Cine
{
	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filepath)
	{
		return CreateRef<OpenGLTexture2D>(filepath);
	}
	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filepath, const TextureSpecification& specification)
	{
		return CreateRef<OpenGLTexture2D>(filepath, specification);
	}
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		return CreateRef<OpenGLTexture2D>(specification);
	}
}