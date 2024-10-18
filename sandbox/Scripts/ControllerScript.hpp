#pragma once
#include "glash/Cine.hpp"

class ControllerScript : Cine::NativeScript
{
public:
	void OnCreate() override
	{
		m_Transform = &GetComponent<Cine::TransformComponent>();
	}

	void OnUpdate(Cine::Timestep ts) override
	{
		float translation = ts * Speed;
		if (Cine::Input::IsKeyPressed(Cine::Key::D))
		{
			m_Transform->Translation.x += translation;
		}
		if (Cine::Input::IsKeyPressed(Cine::Key::A))
		{
			m_Transform->Translation.x -= translation;
		}
		if (Cine::Input::IsKeyPressed(Cine::Key::W))
		{
			m_Transform->Translation.y += translation;
		}
		if (Cine::Input::IsKeyPressed(Cine::Key::S))
		{
			m_Transform->Translation.y -= translation;
		}
	}

private:
	Cine::TransformComponent* m_Transform;

public:
	float Speed = 1.0f;

};