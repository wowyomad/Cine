#include "Components.hpp"

#include "Entity.hpp"

#include <box2d/box2d.h>

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

    void RigidBody2DComponent::SetLinearVelocity(glm::vec2 v)
    {
        static_cast<b2Body*>(RuntimeBody)->SetLinearVelocity({ v.x, v.y });
    }

    void RigidBody2DComponent::SetAngularVelocity(float v)
    {
        static_cast<b2Body*>(RuntimeBody)->SetAngularVelocity(v);
    }


    void RigidBody2DComponent::AddLinearVelocity(glm::vec2 v)
    {
        glm::vec2 newVelocity = GetLinearVelocity() + v;
        SetLinearVelocity(newVelocity);
    }

    void RigidBody2DComponent::AddAngularVelocity(float v)
    {
        float newVelocity = GetAngularVelocity();
        SetAngularVelocity(newVelocity);
    }

    glm::vec2 RigidBody2DComponent::GetLinearVelocity() const
    {
        b2Vec2 b2Vec = static_cast<b2Body*>(RuntimeBody)->GetLinearVelocity();
        return { b2Vec.x, b2Vec.y };
    }

    float RigidBody2DComponent::GetAngularVelocity() const
    {
        return static_cast<b2Body*>(RuntimeBody)->GetAngularVelocity();
    }
}