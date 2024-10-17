#include "glash/glash_pch.hpp"
#include "Sprite.hpp"

namespace Cine
{
	Sprite::Sprite(const Ref<Texture2D>& texture)
		: Texture(texture)
	{

	}

	Sprite::Sprite(const Ref<Texture2D>& texture, const glm::vec2 texCoords[4])
		: Texture(texture)
	{
		for (int i = 0; i < 4; ++i)
			TexCoords[i] = texCoords[i];

	}

	Sprite::Sprite(const Ref<Texture2D>& texture, const Frame& frame)
		: Texture(texture)
	{
		float uMin = static_cast<float>(frame.x) / texture->GetWidth();
		float vMin = static_cast<float>(frame.x) / texture->GetHeight();
		float uMax = static_cast<float>(frame.x + frame.width) / texture->GetWidth();
		float vMax = static_cast<float>(frame.y + frame.height) / texture->GetHeight();

		TexCoords = std::array<glm::vec2, 4>({
			{uMin, vMin},
			{uMax, vMin},
			{uMax, vMax},
			{uMin, vMax}
			});
	}
}