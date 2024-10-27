#include "input.hpp"

KeyInputFunction IsKeyPressedFunc = nullptr;
KeyInputFunction IsKeyDownFunc = nullptr;
KeyInputFunction IsKeyUpFunc = nullptr;
MouseInputFunction IsMouseButtonPressedFunc = nullptr;
MouseInputFunction IsMouseButtonDownFunc = nullptr;
MouseInputFunction IsMouseButtonUpFunc = nullptr;
MousePositionFunction GetMousePositionFunc = nullptr;

bool Input::IsKeyPressed(KeyCode keycode)
{
	return IsKeyPressedFunc ? IsKeyPressedFunc(keycode) : false;
}
bool Input::IsKeyDown(KeyCode keycode)
{
	return IsKeyDownFunc ? IsKeyDownFunc(keycode) : false;
}
bool Input::IsKeyUp(KeyCode keycode)
{
	return IsKeyUpFunc ? IsKeyUpFunc(keycode) : false;

}
bool Input::IsMouseButtonPressed(MouseCode mousecode)
{
	return IsMouseButtonPressedFunc ? IsMouseButtonPressedFunc(mousecode) : false;

}
bool Input::IsMouseButtonUp(MouseCode mousecode)
{
	return IsMouseButtonUpFunc ? IsMouseButtonUpFunc(mousecode) : false;

}
bool Input::IsMouseButtonDown(MouseCode mousecode)
{
	return IsMouseButtonDownFunc ? IsMouseButtonDownFunc(mousecode) : false;

}
glm::vec2 Input::GetMousePosition()
{
	return GetMousePositionFunc ? GetMousePositionFunc() : glm::vec2(0.0f);
}
std::pair<float, float> Input::GetMouseXY()
{
	glm::vec2 position = GetMousePosition();
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