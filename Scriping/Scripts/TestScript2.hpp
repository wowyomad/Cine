#pragma once
#include "glash/Cine.hpp"
#include "glash/Core/Log.hpp"

using namespace Cine;


class TestScript2 : public Cine::NativeScript
{
public:
	void OnCreate()
	{
		std::cout << "TestScript 2 Created" << std::endl;
	}

	void OnUpdate(Timestep ts)
	{
		std::cout << "TestScript 2 Update" << std::endl;

	}


};