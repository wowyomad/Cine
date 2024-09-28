#pragma once
#include "glash/Glash.hpp"



class SimpleLayer : public glash::Layer
{
public:
	SimpleLayer()
		: Layer("Simple Sandbox Layer") {}


	void OnUpdate() override
	{
		GLASH_LOG_TRACE("");
	}

	void OnEvent(glash::Event& event) override
	{
		GLASH_LOG_TRACE("{}", event);
	}


};

class Sandbox : public glash::Application
{
public:
	Sandbox()
	{
		PushLayer(new SimpleLayer());
	}

	~Sandbox()
	{
			
	}
};

