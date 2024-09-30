#pragma once

#include "glash/Core/Core.hpp"
#include "glash/Core/KeyCodes.hpp"
#include "glash/Core/MouseCodes.hpp"


namespace glash
{
	class GLASH_API Input
	{
	public:
		using PlatformKey = int64_t;
		using PlatformMouse = int64_t;

		static KeyCode GlashKeys[512];
		static MouseCode GlashMouseButtons[32];

		static PlatformKey PlatformKeys[512];
		static PlatformMouse PlatformMouseButtons[32];

		static KeyCode ToGlashKey(PlatformKey);
		static MouseCode ToGlashMouse(PlatformMouse);
		static PlatformKey ToPlatformKey(KeyCode);
		static PlatformMouse ToPlatformMouseButton(MouseCode);

		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode mousecode);
		static glm::vec2 GetMousePosition();
		static std::pair<float, float> GetMouseXY();
		static float GetMouseX();
		static float GetMouseY();
		static void Init();
	};
}