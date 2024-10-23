#pragma once
#include "glash/Cine.hpp"
#include "Scene/ComponentSerializer.hpp"

#include <iostream>


using namespace Cine;

class TestScript1 : public Cine::NativeScript
{
public:
	void OnCreate()
	{
		std::cout << "TestScript 2 Created" << std::endl;
	}

	void OnUpdate(Timestep ts)
	{
		std::cout << "TestScript 1 Update" << std::endl;
	}

	SERIALIZE_CLASS(TestScript1)
};