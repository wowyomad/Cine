#pragma once
#include "glash/Cine.hpp"

class ControllerScript : Cine::NativeScript
{
	void OnCreate() override
	{
		m_TransformComponent = &GetComponent<Cine::TransformComponent>();
	}

	void OnUpdate(Cine::Timestep ts) override
	{
		m_TransformComponent->Translation.x += 1 * ts;
	}

	private:
		Cine::TransformComponent* m_TransformComponent;
};