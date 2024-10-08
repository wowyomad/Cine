#pragma once

#include "glash/Core/Core.hpp"

namespace Cine
{
	enum class TextureFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};
	enum class TextureFilter
	{
		Linear = 0,
		Nearest
	};
	enum class TextureWrap
	{
		Repeat = 0,
		ClampToEdge,
		ClampToBorder,
		MirroredRepeat
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		TextureFormat Format = TextureFormat::RGBA8;
		bool GenerateMips = true;

		TextureWrap Wrap = TextureWrap::Repeat;

		TextureFilter MinFilter = TextureFilter::Nearest;
		TextureFilter MagFilter = TextureFilter::Nearest; 

		static inline constexpr TextureSpecification Default2D()
		{
			return TextureSpecification();
		}
	};

	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot) const = 0;

		virtual void SetData(void* data, size_t size) = 0;
		
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;	
		virtual uint32_t GetRendererID() const = 0;
		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual bool IsLoaded() const = 0;
		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D() = default;
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(const TextureSpecification& specification);
		static Ref<Texture2D> Create(const std::filesystem::path& filepath);
		static Ref<Texture2D> Create(const std::filesystem::path& filepath, const TextureSpecification& specification);
	};
}