#pragma once
#include "glash/Cine.hpp"
using namespace Cine;

#include <iostream>


class ControllerScript : public NativeScript
{
public:
	void OnCreate() override
	{
		m_Transform = TryGetComponent<TransformComponent>();
	}

	void OnUpdate(Timestep ts) override
	{
		if (Input::IsKeyPressed(Key::W))
		{
			m_Transform->Translation.y += m_Speed * ts;
		}
		if (Input::IsKeyPressed(Key::S))
		{
			m_Transform->Translation.y -= m_Speed * ts;
		}
		if (Input::IsKeyPressed(Key::D))
		{
			m_Transform->Translation.x += m_Speed * ts;
		}
		if (Input::IsKeyPressed(Key::A))
		{
			m_Transform->Translation.x -= m_Speed * ts;
		}
	}
	SERIALIZE_CLASS(ControllerScript)

private:
	Cine::TransformComponent* m_Transform;
	float m_Speed = 5.0f;
};