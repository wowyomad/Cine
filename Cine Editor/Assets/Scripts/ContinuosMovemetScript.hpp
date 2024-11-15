#pragma once
#include "include/ScriptCore.hpp"
#include "input.hpp"

#include <iostream>

using namespace Cine;

class ContinuosMovemetScript : public NativeScript
{
    public:
        void OnCreate() override
        {
            m_Transform = TryGetComponent<TransformComponent>();
        }

        void OnUpdate(Timestep ts) override
        {
            if(m_Transform)
            {
                if (Input::IsKeyDown(Key::W))
                {
                    m_Direction.y = 1.0f;
                    m_Direction.x = 0.0f;
                }
                else if (Input::IsKeyDown(Key::S))
                {
                    m_Direction.y = -1.0f;
                    m_Direction.x = 0.0f;
                }
                else if (Input::IsKeyDown(Key::D))
                {
                    m_Direction.x = 1.0f;
                    m_Direction.y = 0.0f;
                }
                else if (Input::IsKeyDown(Key::A))
                {
                    m_Direction.x = -1.0f;
                    m_Direction.y = 0.0f;

                }
                else if (Input::IsKeyDown(Key::Space))
                {
                    m_Direction.x = 0.0f;
                    m_Direction.y = 0.0f;
                }

                m_Transform->Translation += m_Direction * m_Speed * ts.Seconds();
            }
        }

        ContinuosMovemetScript& operator=(const ContinuosMovemetScript& other)
        {
            if (this == &other)
                return *this;
                
            m_Speed = other.m_Speed;
            m_Direction = other.m_Direction;

            NativeScript::operator=(other);
            return *this;
        }

    private:

        float m_Speed = 1.0f;
        glm::vec3 m_Direction = {};
        TransformComponent* m_Transform = nullptr;
        SERIALIZE_CLASS(ContinuosMovemetScript,
            FIELD(m_Speed)
            FIELD(m_Direction)
        )
};
