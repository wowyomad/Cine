#include "Components.hpp"

#include "Entity.hpp"

namespace Cine
{
    glm::mat4 TransformComponent::GetWorldTransform(Entity& entity)
    {
        auto& hierarchy = entity.GetComponent<HierarchyComponent>();
        auto& transform = entity.GetComponent<TransformComponent>();

        if (!hierarchy.Parent)
        {
            return transform.GetLocalTransform();
        }

        glm::mat4 parentWorldTransform = GetWorldTransform(hierarchy.Parent);
        return parentWorldTransform * transform.GetLocalTransform();
    }
}