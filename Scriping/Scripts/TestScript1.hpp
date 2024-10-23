#pragma once
#include "glash/Cine.hpp"
#include "Scene/ComponentSerializer.hpp"

#include <iostream>

class TestScript1 : public Cine::NativeScript
{
public:
	void OnCreate() override
	{
		std::cout << "TestScript 2 Created" << std::endl;
	}

	void OnUpdate(Cine::Timestep ts) override
	{
		std::cout << "TestScript 1 Update" << std::endl;
	}

	SERIALIZE_CLASS(TestScript1)
private:
	Cine::TransformComponent* m_Transform;
};