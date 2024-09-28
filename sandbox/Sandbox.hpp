#pragma once
#include "glash/Glash.hpp"



class SimpleLayer : public glash::Layer
{
public:
	SimpleLayer()
		: Layer("Simple Sandbox Layer") {}


	void OnUpdate() override
	{
		
	}

	void OnEvent(glash::Event& event) override
	{
		
	}


};

class Sandbox : public glash::Application
{
public:
	Sandbox()
	{
		PushLayer(new SimpleLayer());
		PushLayer(new glash::ImGuiLayer());
	}

	~Sandbox()
	{
			
	}
};

