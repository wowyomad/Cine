#pragma once
#include "glash/Glash.hpp"
#include "glash/Input.hpp"

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

	void OnImGuiRender() override
	{	
		ImGui::Begin(m_Name.c_str());
		ImGui::Text("Fuck this shit");
		ImGui::End();
	}

private:
	char m_Buf[1024]{};


};

class Sandbox : public glash::Application
{
public:
	Sandbox()
	{
		PushOverlay(new SimpleLayer());
	}

	~Sandbox()
	{
			
	}
};

