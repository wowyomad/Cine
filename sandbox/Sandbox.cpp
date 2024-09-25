#include "Sandbox.hpp"

#include "glash/Core.hpp"
#include "glash/Glash.hpp"

glash::Application* glash::CreateApplication()
{
	GLASH_TRACE("Trace inside {}", __FUNCTION__);
	GLASH_DEBUG("Debug inside {}", __FUNCTION__);
	GLASH_INFO("Info inside {}", __FUNCTION__);
	GLASH_ERROR("Error inside {}", __FUNCTION__);

	return new Sandbox();
}