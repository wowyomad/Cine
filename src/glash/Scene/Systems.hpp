#pragma once

#include "glash/Core/Timestep.hpp"

#include <entt/entt.hpp>

namespace Cine
{
	class SpriteRendererSystem
	{
	public:
		static void Update(entt::registry& registry);
	};

	class SpriteAnimationSystem
	{
	public:
		static void Update(entt::registry& registry, Timestep deltaTime);
	};
}