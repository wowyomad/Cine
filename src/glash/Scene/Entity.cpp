#include "glash/glash_pch.hpp"
#include "Entity.hpp"

namespace Cine
{
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
        CINE_CORE_ASSERT(scene != nullptr && handle != entt::null, "That won't work.");
    }
}