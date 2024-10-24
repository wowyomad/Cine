#pragma once
#include "Cine.hpp"

using namespace Cine;

class MoverScript : public NativeScript
{
    public:
        void OnCreate()
        {
            m_Transform = TryGetComponent<TransformComponent>();
        }

        void OnUpdate(Timestep ts)
        {
            
        }

    private:
        float m_MoveDistanceLeft = -10.0f;
        float m_MoveDistanceRight = 10.0f;
        float m_Speed = 1.0f;
        TransformComponent* m_Transform = nullptr;

        SERIALIZE_CLASS(MoverScript,
            FIELD(m_MoveDistanceLeft)
            FIELD(m_MoveDistanceRight)
            FIELD(m_Speed)
        )
};