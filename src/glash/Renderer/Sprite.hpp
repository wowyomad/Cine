#pragma once

#include "glash/Core/Base.hpp"
#include "glash/Renderer/Texture.hpp"

namespace Cine
{
	class Sprite
	{
	public:
		struct Frame
		{
			int x, y;
			int width, height;
		};

	public:
		Sprite(const Ref<Texture2D>& texture);
		Sprite(const Ref<Texture2D>& texture, const glm::vec2 texCoords[4]);
		Sprite(const Ref<Texture2D>& texture, const Frame& frame);

	public:
		Ref<Texture2D> Texture;
		std::array<glm::vec2, 4> TexCoords;
	};
	
};