#pragma once
#include "include/ScriptCore.hpp"

using namespace Cine;

class SelfDestructionScript : public NativeScript
{
public:
    SelfDestructionScript() = default;
    SelfDestructionScript(float destructionTime) : TimeToDestruction(destructionTime) {}

    void OnCreate() override
    {
        CINE_LOG_INFO("Constructed {0:0x} for {1}", (size_t)this, (uint32_t)m_Entity);
    }

    void OnUpdate(Timestep ts) override
    {
        m_Time += ts;
        int id = (uint32_t)m_Entity;
        if (id == 8)
        {
            std::cout << "Destroyed " << Destroyed << std::endl;
        }
        if (!Destroyed && m_Time >= TimeToDestruction)
        {

            std::cout << "\n";
            CINE_LOG_WARN("Self Destructing entity {0} in {1:0x}", (uint32_t)m_Entity, (size_t)this);
            std::cout << "\n";
            m_Entity.Destroy();
            Destroyed = true;
        }
    }

    // Assign fields that you expect to be serialized (don't assign pointers or references!)
    SelfDestructionScript& operator=(const SelfDestructionScript& other)
    {
        if (this == &other)
            return *this;

        m_Time = other.m_Time;
        TimeToDestruction = other.TimeToDestruction;
        Destroyed = other.Destroyed;

        NativeScript::operator=(other);
        return *this;
    }

private:
    float m_Time = 0.0f;
    float TimeToDestruction = 10.0f;
    bool Destroyed = false;

    SERIALIZE_CLASS(SelfDestructionScript,
        FIELD(m_Time)
        FIELD(TimeToDestruction)
        FIELD(Destroyed)
    )
};
