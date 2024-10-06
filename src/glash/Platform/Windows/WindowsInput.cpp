#include "glash/glash_pch.hpp"
#include "glash/Core/Core.hpp"

#include "glash/Core/Input.hpp"
#include "glash/Core/KeyCodes.hpp"
#include "glash/Core/MouseCodes.hpp"
#include "glash/Core/Application.hpp"

#include <GLFW/glfw3.h>

#if defined(GLASH_PLATFORM_WINDOWS) == 1
namespace Cine
{
	class Input;

	using PlatformKey = Input::PlatformKey;
	using PlatformMouse = Input::PlatformMouse;


	KeyCode Input::GlashKeys[512] {};
	MouseCode Input::GlashMouseButtons[32]{};

	Input::PlatformKey Input::PlatformKeys[512]{};
	Input::PlatformMouse Input::PlatformMouseButtons[32]{};

	Input::KeyState Input::KeyStates[512]{};
	Input::KeyState Input::MouseButtonStates[32]{};


	KeyCode Input::ToGlashKey(Input::PlatformKey glfwKeyCode)
	{
		return Input::GlashKeys[glfwKeyCode];
	}
	MouseCode Input::ToGlashMouse(PlatformMouse glfwMouseCode)
	{
		return Input::GlashMouseButtons[glfwMouseCode];
	}

	PlatformKey Input::ToPlatformKey(KeyCode keyCode)
	{
		return Input::PlatformKeys[keyCode];
	}
	PlatformMouse Input::ToPlatformMouseButton(MouseCode mouse)
	{
		return Input::PlatformMouseButtons[mouse];
	}

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, ToPlatformKey(keycode));
		return state == GLFW_PRESS;
	}

	bool Input::IsKeyDown(KeyCode keycode)
	{
		return KeyStates[keycode] == KeyState::Down;
	}

	bool Input::IsKeyUp(KeyCode keycode)
	{
		return KeyStates[keycode] == KeyState::Up;
	}

	void Input::SetKey(KeyCode keycode, KeyState state)
	{
		KeyStates[keycode] = state;
	}

	void Input::ClearKeyStates()
	{
		std::memset(KeyStates, static_cast<int>(KeyState::Idle), sizeof(KeyStates));
	}


	bool Input::IsMouseButtonPressed(const MouseCode mouse)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, ToPlatformMouseButton(mouse));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { static_cast<float>(xpos), static_cast<float>(ypos) };
	}

	std::pair<float, float> Input::GetMouseXY()
	{
		const glm::vec2 position = GetMousePosition();
		return { position.x, position.y };
	}


	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

	void Input::Init()
	{
		GlashKeys[GLFW_KEY_SPACE] = Key::Space;
		GlashKeys[GLFW_KEY_APOSTROPHE] = Key::Apostrophe;
		GlashKeys[GLFW_KEY_COMMA] = Key::Comma;
		GlashKeys[GLFW_KEY_MINUS] = Key::Minus;
		GlashKeys[GLFW_KEY_PERIOD] = Key::Period;
		GlashKeys[GLFW_KEY_SLASH] = Key::Slash;

		GlashKeys[GLFW_KEY_0] = Key::D0;
		GlashKeys[GLFW_KEY_1] = Key::D1;
		GlashKeys[GLFW_KEY_2] = Key::D2;
		GlashKeys[GLFW_KEY_3] = Key::D3;
		GlashKeys[GLFW_KEY_4] = Key::D4;
		GlashKeys[GLFW_KEY_5] = Key::D5;
		GlashKeys[GLFW_KEY_6] = Key::D6;
		GlashKeys[GLFW_KEY_7] = Key::D7;
		GlashKeys[GLFW_KEY_8] = Key::D8;
		GlashKeys[GLFW_KEY_9] = Key::D9;

		GlashKeys[GLFW_KEY_SEMICOLON] = Key::Semicolon;
		GlashKeys[GLFW_KEY_EQUAL] = Key::Equal;

		GlashKeys[GLFW_KEY_A] = Key::A;
		GlashKeys[GLFW_KEY_B] = Key::B;
		GlashKeys[GLFW_KEY_C] = Key::C;
		GlashKeys[GLFW_KEY_D] = Key::D;
		GlashKeys[GLFW_KEY_E] = Key::E;
		GlashKeys[GLFW_KEY_F] = Key::F;
		GlashKeys[GLFW_KEY_G] = Key::G;
		GlashKeys[GLFW_KEY_H] = Key::H;
		GlashKeys[GLFW_KEY_I] = Key::I;
		GlashKeys[GLFW_KEY_J] = Key::J;
		GlashKeys[GLFW_KEY_K] = Key::K;
		GlashKeys[GLFW_KEY_L] = Key::L;
		GlashKeys[GLFW_KEY_M] = Key::M;
		GlashKeys[GLFW_KEY_N] = Key::N;
		GlashKeys[GLFW_KEY_O] = Key::O;
		GlashKeys[GLFW_KEY_P] = Key::P;
		GlashKeys[GLFW_KEY_Q] = Key::Q;
		GlashKeys[GLFW_KEY_R] = Key::R;
		GlashKeys[GLFW_KEY_S] = Key::S;
		GlashKeys[GLFW_KEY_T] = Key::T;
		GlashKeys[GLFW_KEY_U] = Key::U;
		GlashKeys[GLFW_KEY_V] = Key::V;
		GlashKeys[GLFW_KEY_W] = Key::W;
		GlashKeys[GLFW_KEY_X] = Key::X;
		GlashKeys[GLFW_KEY_Y] = Key::Y;
		GlashKeys[GLFW_KEY_Z] = Key::Z;

		GlashKeys[GLFW_KEY_LEFT_BRACKET] = Key::LeftBracket;
		GlashKeys[GLFW_KEY_BACKSLASH] = Key::Backslash;
		GlashKeys[GLFW_KEY_RIGHT_BRACKET] = Key::RightBracket;
		GlashKeys[GLFW_KEY_GRAVE_ACCENT] = Key::GraveAccent;

		GlashKeys[GLFW_KEY_WORLD_1] = Key::World1;
		GlashKeys[GLFW_KEY_WORLD_2] = Key::World2;

		/* Function keys */
		GlashKeys[GLFW_KEY_ESCAPE] = Key::Escape;
		GlashKeys[GLFW_KEY_ENTER] = Key::Enter;
		GlashKeys[GLFW_KEY_TAB] = Key::Tab;
		GlashKeys[GLFW_KEY_BACKSPACE] = Key::Backspace;
		GlashKeys[GLFW_KEY_INSERT] = Key::Insert;
		GlashKeys[GLFW_KEY_DELETE] = Key::Delete;
		GlashKeys[GLFW_KEY_RIGHT] = Key::Right;
		GlashKeys[GLFW_KEY_LEFT] = Key::Left;
		GlashKeys[GLFW_KEY_DOWN] = Key::Down;
		GlashKeys[GLFW_KEY_UP] = Key::Up;
		GlashKeys[GLFW_KEY_PAGE_UP] = Key::PageUp;
		GlashKeys[GLFW_KEY_PAGE_DOWN] = Key::PageDown;
		GlashKeys[GLFW_KEY_HOME] = Key::Home;
		GlashKeys[GLFW_KEY_END] = Key::End;
		GlashKeys[GLFW_KEY_CAPS_LOCK] = Key::CapsLock;
		GlashKeys[GLFW_KEY_SCROLL_LOCK] = Key::ScrollLock;
		GlashKeys[GLFW_KEY_NUM_LOCK] = Key::NumLock;
		GlashKeys[GLFW_KEY_PRINT_SCREEN] = Key::PrintScreen;
		GlashKeys[GLFW_KEY_PAUSE] = Key::Pause;
		GlashKeys[GLFW_KEY_F1] = Key::F1;
		GlashKeys[GLFW_KEY_F2] = Key::F2;
		GlashKeys[GLFW_KEY_F3] = Key::F3;
		GlashKeys[GLFW_KEY_F4] = Key::F4;
		GlashKeys[GLFW_KEY_F5] = Key::F5;
		GlashKeys[GLFW_KEY_F6] = Key::F6;
		GlashKeys[GLFW_KEY_F7] = Key::F7;
		GlashKeys[GLFW_KEY_F8] = Key::F8;
		GlashKeys[GLFW_KEY_F9] = Key::F9;
		GlashKeys[GLFW_KEY_F10] = Key::F10;
		GlashKeys[GLFW_KEY_F11] = Key::F11;
		GlashKeys[GLFW_KEY_F12] = Key::F12;
		GlashKeys[GLFW_KEY_F13] = Key::F13;
		GlashKeys[GLFW_KEY_F14] = Key::F14;
		GlashKeys[GLFW_KEY_F15] = Key::F15;
		GlashKeys[GLFW_KEY_F16] = Key::F16;
		GlashKeys[GLFW_KEY_F17] = Key::F17;
		GlashKeys[GLFW_KEY_F18] = Key::F18;
		GlashKeys[GLFW_KEY_F19] = Key::F19;
		GlashKeys[GLFW_KEY_F20] = Key::F20;
		GlashKeys[GLFW_KEY_F21] = Key::F21;
		GlashKeys[GLFW_KEY_F22] = Key::F22;
		GlashKeys[GLFW_KEY_F23] = Key::F23;
		GlashKeys[GLFW_KEY_F24] = Key::F24;
		GlashKeys[GLFW_KEY_F25] = Key::F25;

		/* Keypad */
		GlashKeys[GLFW_KEY_KP_0] = Key::KP0;
		GlashKeys[GLFW_KEY_KP_1] = Key::KP1;
		GlashKeys[GLFW_KEY_KP_2] = Key::KP2;
		GlashKeys[GLFW_KEY_KP_3] = Key::KP3;
		GlashKeys[GLFW_KEY_KP_4] = Key::KP4;
		GlashKeys[GLFW_KEY_KP_5] = Key::KP5;
		GlashKeys[GLFW_KEY_KP_6] = Key::KP6;
		GlashKeys[GLFW_KEY_KP_7] = Key::KP7;
		GlashKeys[GLFW_KEY_KP_8] = Key::KP8;
		GlashKeys[GLFW_KEY_KP_9] = Key::KP9;
		GlashKeys[GLFW_KEY_KP_DECIMAL] = Key::KPDecimal;
		GlashKeys[GLFW_KEY_KP_DIVIDE] = Key::KPDivide;
		GlashKeys[GLFW_KEY_KP_MULTIPLY] = Key::KPMultiply;
		GlashKeys[GLFW_KEY_KP_SUBTRACT] = Key::KPSubtract;
		GlashKeys[GLFW_KEY_KP_ADD] = Key::KPAdd;
		GlashKeys[GLFW_KEY_KP_ENTER] = Key::KPEnter;
		GlashKeys[GLFW_KEY_KP_EQUAL] = Key::KPEqual;

		GlashKeys[GLFW_KEY_LEFT_SHIFT] = Key::LeftShift;
		GlashKeys[GLFW_KEY_LEFT_CONTROL] = Key::LeftControl;
		GlashKeys[GLFW_KEY_LEFT_ALT] = Key::LeftAlt;
		GlashKeys[GLFW_KEY_LEFT_SUPER] = Key::LeftSuper;
		GlashKeys[GLFW_KEY_RIGHT_SHIFT] = Key::RightShift;
		GlashKeys[GLFW_KEY_RIGHT_CONTROL] = Key::RightControl;
		GlashKeys[GLFW_KEY_RIGHT_ALT] = Key::RightAlt;
		GlashKeys[GLFW_KEY_RIGHT_SUPER] = Key::RightSuper;
		GlashKeys[GLFW_KEY_MENU] = Key::Menu;

		GlashMouseButtons[GLFW_MOUSE_BUTTON_1] = Mouse::Button0;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_2] = Mouse::Button1;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_3] = Mouse::Button2;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_4] = Mouse::Button3;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_5] = Mouse::Button4;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_6] = Mouse::Button5;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_7] = Mouse::Button6;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_8] = Mouse::Button7;

		GlashMouseButtons[GLFW_MOUSE_BUTTON_LAST] = Mouse::ButtonLast;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_LEFT] = Mouse::ButtonLeft;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_RIGHT] = Mouse::ButtonRight;
		GlashMouseButtons[GLFW_MOUSE_BUTTON_MIDDLE] = Mouse::ButtonMiddle;

		PlatformKeys[Key::Space] = GLFW_KEY_SPACE;
		PlatformKeys[Key::Apostrophe] = GLFW_KEY_APOSTROPHE;
		PlatformKeys[Key::Comma] = GLFW_KEY_COMMA;
		PlatformKeys[Key::Minus] = GLFW_KEY_MINUS;
		PlatformKeys[Key::Period] = GLFW_KEY_PERIOD;
		PlatformKeys[Key::Slash] = GLFW_KEY_SLASH;

		PlatformKeys[Key::D0] = GLFW_KEY_0;
		PlatformKeys[Key::D1] = GLFW_KEY_1;
		PlatformKeys[Key::D2] = GLFW_KEY_2;
		PlatformKeys[Key::D3] = GLFW_KEY_3;
		PlatformKeys[Key::D4] = GLFW_KEY_4;
		PlatformKeys[Key::D5] = GLFW_KEY_5;
		PlatformKeys[Key::D6] = GLFW_KEY_6;
		PlatformKeys[Key::D7] = GLFW_KEY_7;
		PlatformKeys[Key::D8] = GLFW_KEY_8;
		PlatformKeys[Key::D9] = GLFW_KEY_9;

		PlatformKeys[Key::Semicolon] = GLFW_KEY_SEMICOLON;
		PlatformKeys[Key::Equal] = GLFW_KEY_EQUAL;

		PlatformKeys[Key::A] = GLFW_KEY_A;
		PlatformKeys[Key::B] = GLFW_KEY_B;
		PlatformKeys[Key::C] = GLFW_KEY_C;
		PlatformKeys[Key::D] = GLFW_KEY_D;
		PlatformKeys[Key::E] = GLFW_KEY_E;
		PlatformKeys[Key::F] = GLFW_KEY_F;
		PlatformKeys[Key::G] = GLFW_KEY_G;
		PlatformKeys[Key::H] = GLFW_KEY_H;
		PlatformKeys[Key::I] = GLFW_KEY_I;
		PlatformKeys[Key::J] = GLFW_KEY_J;
		PlatformKeys[Key::K] = GLFW_KEY_K;
		PlatformKeys[Key::L] = GLFW_KEY_L;
		PlatformKeys[Key::M] = GLFW_KEY_M;
		PlatformKeys[Key::N] = GLFW_KEY_N;
		PlatformKeys[Key::O] = GLFW_KEY_O;
		PlatformKeys[Key::P] = GLFW_KEY_P;
		PlatformKeys[Key::Q] = GLFW_KEY_Q;
		PlatformKeys[Key::R] = GLFW_KEY_R;
		PlatformKeys[Key::S] = GLFW_KEY_S;
		PlatformKeys[Key::T] = GLFW_KEY_T;
		PlatformKeys[Key::U] = GLFW_KEY_U;
		PlatformKeys[Key::V] = GLFW_KEY_V;
		PlatformKeys[Key::W] = GLFW_KEY_W;
		PlatformKeys[Key::X] = GLFW_KEY_X;
		PlatformKeys[Key::Y] = GLFW_KEY_Y;
		PlatformKeys[Key::Z] = GLFW_KEY_Z;

		PlatformKeys[Key::LeftBracket] = GLFW_KEY_LEFT_BRACKET;
		PlatformKeys[Key::Backslash] = GLFW_KEY_BACKSLASH;
		PlatformKeys[Key::RightBracket] = GLFW_KEY_RIGHT_BRACKET;
		PlatformKeys[Key::GraveAccent] = GLFW_KEY_GRAVE_ACCENT;

		PlatformKeys[Key::World1] = GLFW_KEY_WORLD_1;
		PlatformKeys[Key::World2] = GLFW_KEY_WORLD_2;

		// Function keys
		PlatformKeys[Key::Escape] = GLFW_KEY_ESCAPE;
		PlatformKeys[Key::Enter] = GLFW_KEY_ENTER;
		PlatformKeys[Key::Tab] = GLFW_KEY_TAB;
		PlatformKeys[Key::Backspace] = GLFW_KEY_BACKSPACE;
		PlatformKeys[Key::Insert] = GLFW_KEY_INSERT;
		PlatformKeys[Key::Delete] = GLFW_KEY_DELETE;
		PlatformKeys[Key::Right] = GLFW_KEY_RIGHT;
		PlatformKeys[Key::Left] = GLFW_KEY_LEFT;
		PlatformKeys[Key::Down] = GLFW_KEY_DOWN;
		PlatformKeys[Key::Up] = GLFW_KEY_UP;
		PlatformKeys[Key::PageUp] = GLFW_KEY_PAGE_UP;
		PlatformKeys[Key::PageDown] = GLFW_KEY_PAGE_DOWN;
		PlatformKeys[Key::Home] = GLFW_KEY_HOME;
		PlatformKeys[Key::End] = GLFW_KEY_END;
		PlatformKeys[Key::CapsLock] = GLFW_KEY_CAPS_LOCK;
		PlatformKeys[Key::ScrollLock] = GLFW_KEY_SCROLL_LOCK;
		PlatformKeys[Key::NumLock] = GLFW_KEY_NUM_LOCK;
		PlatformKeys[Key::PrintScreen] = GLFW_KEY_PRINT_SCREEN;
		PlatformKeys[Key::Pause] = GLFW_KEY_PAUSE;
		PlatformKeys[Key::F1] = GLFW_KEY_F1;
		PlatformKeys[Key::F2] = GLFW_KEY_F2;
		PlatformKeys[Key::F3] = GLFW_KEY_F3;
		PlatformKeys[Key::F4] = GLFW_KEY_F4;
		PlatformKeys[Key::F5] = GLFW_KEY_F5;
		PlatformKeys[Key::F6] = GLFW_KEY_F6;
		PlatformKeys[Key::F7] = GLFW_KEY_F7;
		PlatformKeys[Key::F8] = GLFW_KEY_F8;
		PlatformKeys[Key::F9] = GLFW_KEY_F9;
		PlatformKeys[Key::F10] = GLFW_KEY_F10;
		PlatformKeys[Key::F11] = GLFW_KEY_F11;
		PlatformKeys[Key::F12] = GLFW_KEY_F12;

		// Keypad keys
		PlatformKeys[Key::KP0] = GLFW_KEY_KP_0;
		PlatformKeys[Key::KP1] = GLFW_KEY_KP_1;
		PlatformKeys[Key::KP2] = GLFW_KEY_KP_2;
		PlatformKeys[Key::KP3] = GLFW_KEY_KP_3;
		PlatformKeys[Key::KP4] = GLFW_KEY_KP_4;
		PlatformKeys[Key::KP5] = GLFW_KEY_KP_5;
		PlatformKeys[Key::KP6] = GLFW_KEY_KP_6;
		PlatformKeys[Key::KP7] = GLFW_KEY_KP_7;
		PlatformKeys[Key::KP8] = GLFW_KEY_KP_8;
		PlatformKeys[Key::KP9] = GLFW_KEY_KP_9;
		PlatformKeys[Key::KPDecimal] = GLFW_KEY_KP_DECIMAL;
		PlatformKeys[Key::KPDivide] = GLFW_KEY_KP_DIVIDE;
		PlatformKeys[Key::KPMultiply] = GLFW_KEY_KP_MULTIPLY;
		PlatformKeys[Key::KPSubtract] = GLFW_KEY_KP_SUBTRACT;
		PlatformKeys[Key::KPAdd] = GLFW_KEY_KP_ADD;
		PlatformKeys[Key::KPEnter] = GLFW_KEY_KP_ENTER;
		PlatformKeys[Key::KPEqual] = GLFW_KEY_KP_EQUAL;

		PlatformKeys[Key::LeftShift] = GLFW_KEY_LEFT_SHIFT;
		PlatformKeys[Key::LeftControl] = GLFW_KEY_LEFT_CONTROL;
		PlatformKeys[Key::LeftAlt] = GLFW_KEY_LEFT_ALT;
		PlatformKeys[Key::LeftSuper] = GLFW_KEY_LEFT_SUPER;
		PlatformKeys[Key::RightShift] = GLFW_KEY_RIGHT_SHIFT;
		PlatformKeys[Key::RightControl] = GLFW_KEY_RIGHT_CONTROL;
		PlatformKeys[Key::RightAlt] = GLFW_KEY_RIGHT_ALT;
		PlatformKeys[Key::RightSuper] = GLFW_KEY_RIGHT_SUPER;
		PlatformKeys[Key::Menu] = GLFW_KEY_MENU;

		// Mapping from glash::Mouse to GLFW mouse buttons
		PlatformMouseButtons[Mouse::Button0] = GLFW_MOUSE_BUTTON_1;
		PlatformMouseButtons[Mouse::Button1] = GLFW_MOUSE_BUTTON_2;
		PlatformMouseButtons[Mouse::Button2] = GLFW_MOUSE_BUTTON_3;
		PlatformMouseButtons[Mouse::Button3] = GLFW_MOUSE_BUTTON_4;
		PlatformMouseButtons[Mouse::Button4] = GLFW_MOUSE_BUTTON_5;
		PlatformMouseButtons[Mouse::Button5] = GLFW_MOUSE_BUTTON_6;
		PlatformMouseButtons[Mouse::Button6] = GLFW_MOUSE_BUTTON_7;
		PlatformMouseButtons[Mouse::Button7] = GLFW_MOUSE_BUTTON_8;

		PlatformMouseButtons[Mouse::ButtonLast] = GLFW_MOUSE_BUTTON_LAST;
		PlatformMouseButtons[Mouse::ButtonLeft] = GLFW_MOUSE_BUTTON_LEFT;
		PlatformMouseButtons[Mouse::ButtonRight] = GLFW_MOUSE_BUTTON_RIGHT;
		PlatformMouseButtons[Mouse::ButtonMiddle] = GLFW_MOUSE_BUTTON_MIDDLE;
	}
}
#endif
