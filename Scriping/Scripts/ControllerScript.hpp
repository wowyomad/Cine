#pragma once
#include "glash/Cine.hpp"
#include "Scene/ComponentSerializer.hpp"

#include <iostream>

using namespace Cine;


class ControllerScript : public NativeScript
{
public:
	void OnCreate() override
	{
		m_Transform = TryGetComponent<TransformComponent>();
	}

	void OnDestroy() override
	{

	}

	void OnUpdate(Timestep ts) override
	{
		if (!m_Transform)
		{
			return;
		}

		float speed = 5.0f;
		glm::vec3 direction(0.0f);

		if (Input::IsKeyPressed(Key::D))
		{
			direction.x += 1.0f;
		}
		if (Input::IsKeyPressed(Key::A))
		{
			direction.x -= 1.0f;
		}
		if (Input::IsKeyPressed(Key::W))
		{
			direction.y += 1.0f;
		}
		if (Input::IsKeyPressed(Key::S))
		{
			direction.y -= 1.0f;
		}

		if (glm::length(direction) == 0.0f)
		{
			
		}

		m_Transform->Translation += speed * ts * direction;

	}

	SERIALIZE_CLASS(ControllerScript)

private:
	TransformComponent* m_Transform;
};