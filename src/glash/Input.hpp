#pragma once

#include "glash/Core.hpp"
#include "glash/KeyCodes.hpp"
#include "glash/MouseCodes.hpp"


namespace glash
{
	class GLASH_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode mousecode);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}