#pragma once

#include "glash/Cine.hpp"
#include "Sandbox2D.hpp"

class SandboxApp : public Cine::Application
{
public:
	SandboxApp() : Application("Sandbox2D")
	{
		PushLayer(new Sandbox2D(this));
	}
};

