#pragma once
#include "glash/Core/KeyCodes.hpp"
#include "glash/Core/MouseCodes.hpp"

#include "input.hpp"

using namespace Cine;

#include <glm/glm.hpp>

using KeyInputFunction = bool(*)(KeyCode);
using MouseInputFunction = bool(*)(MouseCode);
using MousePositionFunction = glm::vec2(*)();

extern KeyInputFunction IsKeyPressedFunc;
extern KeyInputFunction IsKeyDownFunc;
extern KeyInputFunction IsKeyUpFunc;
extern MouseInputFunction IsMouseButtonPressedFunc;
extern MouseInputFunction IsMouseButtonDownFunc;
extern MouseInputFunction IsMouseButtonUpFunc;
extern MousePositionFunction GetMousePositionFunc;

class Input
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

	static bool IsKeyPressed(KeyCode keycode);
	static bool IsKeyDown(KeyCode keycode);
	static bool IsKeyUp(KeyCode keycode);
	static bool IsMouseButtonPressed(MouseCode mousecode);
	static bool IsMouseButtonUp(MouseCode mousecode);
	static bool IsMouseButtonDown(MouseCode mousecode);
	static glm::vec2 GetMousePosition();
	static std::pair<float, float> GetMouseXY();
	static float GetMouseX();
	static float GetMouseY();
};