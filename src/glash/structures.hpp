#pragma once
#include "glash_pch.hpp"

namespace glash
{
	using Color = glm::vec4;
	namespace color {
		constexpr Color RED = Color(255, 0, 0, 0);
		constexpr Color GREEN = Color(0, 255, 0, 0);
		constexpr Color BLUE = Color(0, 0, 255, 0);
	}
}