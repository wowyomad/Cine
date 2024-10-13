#include "glash/glash_pch.hpp"
#include "Entity.hpp"

namespace Cine
{
    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {

    }
}