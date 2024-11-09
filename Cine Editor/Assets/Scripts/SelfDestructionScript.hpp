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
            
        }

        void OnUpdate(Timestep ts) override
        {
            m_Time += ts;

            if (!Destroyed && m_Time >= TimeToDestruction)
            {
                m_Entity.Destroy();
            }
        }

        // Assign fields that you expect to be serialized (don't assign pointers or references!)
        SelfDestructionScript& operator=(const SelfDestructionScript& other)
        {
            if (this == &other)
                return *this;
            m_Time = other.m_Time;
            TimeToDestruction = other.TimeToDestruction;

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
