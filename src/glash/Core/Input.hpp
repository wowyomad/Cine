#pragma once

#include "glash/Core/Base.hpp"
#include "glash/Core/KeyCodes.hpp"
#include "glash/Core/MouseCodes.hpp"


namespace Cine
{
	class GLASH_API Input
	{
	public:
		using PlatformKey = int64_t;
		using PlatformMouse = int64_t;

		enum class KeyState : uint8_t
		{
			Idle = 0,
			Up = 1,
			Down = 2
		};

		static KeyCode GlashKeys[512];
		static MouseCode GlashMouseButtons[32];

		static PlatformKey PlatformKeys[512];
		static PlatformMouse PlatformMouseButtons[32];

		static KeyState KeyStates[512];
		static KeyState MouseButtonStates[32];

		static KeyCode ToCineKey(PlatformKey);
		static MouseCode ToCineMouse(PlatformMouse);
		static PlatformKey ToPlatformKey(KeyCode);
		static PlatformMouse ToPlatformMouseButton(MouseCode);

		static void Init();

		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyDown(KeyCode keycode);
		static bool IsKeyUp(KeyCode keycode);
		static void SetKey(KeyCode keycode, KeyState state);

		static void ClearStates();

		static bool IsMouseButtonPressed(MouseCode mousecode);
		static bool IsMouseButtonUp(MouseCode mousecode);
		static bool IsMouseButtonDown(MouseCode mousecode);
		static void SetMouseButton(MouseCode mousecode, KeyState state);

		static glm::vec2 GetMousePosition();
		static std::pair<float, float> GetMouseXY();
		static float GetMouseX();
		static float GetMouseY();
	};
}