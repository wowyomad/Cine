#pragma once
#include "include/Base.hpp"
using namespace Cine;
    
class RotatorScript : public NativeScript
{
public:
    void OnCreate()
    {
        m_Transform = TryGetComponent<TransformComponent>();
    }

    void OnUpdate(Timestep ts)
    {
        if(m_Transform)
        {
            m_Transform->Rotation.z += glm::radians(m_RotationSpeed) * ts;
        }
    }

    RotatorScript& operator=(const RotatorScript& other)
    {
        m_RotationSpeed = other.m_RotationSpeed;
        return *this;
    }

private:
    float m_RotationSpeed = 10.0f;
    TransformComponent* m_Transform = nullptr;

    SERIALIZE_CLASS(RotatorScript,
        FIELD(m_RotationSpeed)
    )

};