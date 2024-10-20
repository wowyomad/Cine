#include "Components.hpp"

#include "Entity.hpp"

namespace Cine
{
	glm::mat4 TransformComponent::GetWorldTransform(const Entity& entity)
	{
		CINE_CORE_ASSERT(false, "Not implemented yet");
		return glm::mat4(1.0f);
	}
}