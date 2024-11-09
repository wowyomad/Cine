#pragma once
#include "include/ScriptCore.hpp"

using namespace Cine;

class BodySpawnerScript : public NativeScript
{
    public:
        void OnCreate() override
        {
            
        }

        void SpawnEntity()
        {
                Entity entity = CreateEntity();
                entity.LocalTranslation() = GetCom
                entity.AddComponent<BoxCollider2DComponent>();
                entity.AddComponent<SpriteRendererComponent>();
        }

        void OnUpdate(Timestep ts) override
        {
            m_TimePassed += ts;

            if(m_TimePassed >= Interval)
            {
                SpawnEntity();
               
                m_TimePassed = 0.0f;
            }
        }



        // Assign fields that you expect to be serialized (don't assign pointers or references!)
        BodySpawnerScript& operator=(const BodySpawnerScript& other)
        {
            if (this == &other)
                return *this;
            
            Interval = other.Interval;

            return *this;
        }

    private:

        float Interval = 5.0f;

        float m_TimePassed = 0.0f;

        SERIALIZE_CLASS(BodySpawnerScript,
            FIELD(Interval)
            FIELD(m_TimePassed)
        )
};
