#include "Sandbox.hpp"

#include "glash/Log.hpp"
#include "glash/Glash.hpp"

glash::Application* glash::CreateApplication()
{
	return new Sandbox();
}