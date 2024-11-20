#pragma once
#include "include/ScriptCore.hpp"

using namespace Cine;

class PController2D : public NativeScript
{
    public:
        void OnCreate() override
        {
            if(HasComponent<RigidBody2DComponent>())
            {
                if (GetComponent<RigidBody2DComponent>().RuntimeBody)
                {
                    static_cast<b2Body*>(GetComponent<RigidBody2DComponent>().RuntimeBody)->SetGravityScale(0.0f);

                }
            }

        }

        void OnUpdate(Timestep ts) override
        {
            if(Object.HasComponent<RigidBody2DComponent>())
            {
                glm::vec2 direction = {};
                
                if (Input::IsKeyPressed(Key::W))
                {
                    direction.y += 1.0f;
                }
                if (Input::IsKeyPressed(Key::S))
                {
                    direction.y -= 1.0f;
                }
                if (Input::IsKeyPressed(Key::D))
                {
                    direction.x += 1.0f;
                }
                if (Input::IsKeyPressed(Key::A))
                {
                    direction.x -= 1.0f;
                }

                if(direction.x > 0 && direction.y > 0)
                {
                    direction = glm::normalize(direction);
                }

                Object.GetComponent<RigidBody2DComponent>().SetLinearVelocity(direction * Speed);
                Object.GetComponent<RigidBody2DComponent>().SetAngularVelocity(0.0f);

            }
        }

        // Assign fields that you expect to be serialized (don't assign pointers or references!)
        PController2D& operator=(const PController2D& other)
        {
            if (this == &other)
                return *this;

            Speed = other.Speed;

            NativeScript::operator=(other);
            return *this;
        }

    public:
        float Speed = 5.0f;


    private:
        SERIALIZE_CLASS(PController2D,
            FIELD(Speed)
        )
};
