#pragma once
#include "glash/Cine.hpp"
#include <iostream>

class TestScript : public Cine::NativeScript
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

	SERIALIZE_CLASS(TestScript,
		FIELD(m_Number)
	)
private:
	Cine::TransformComponent* m_Transform;
	int m_Number = 0;
};