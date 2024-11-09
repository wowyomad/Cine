#pragma once
#include "include/ScriptCore.hpp"

#include "SelfDestructionScript.hpp"

using namespace Cine;

class BodySpawnerScript : public NativeScript
{
public:
    void OnCreate() override
    {

    }

    void SpawnEntity(std::string name = "")
    {
        Entity entity = CreateEntity();
        entity.LocalTranslation() = Translation();

        entity.AddComponent<BoxCollider2DComponent>();
        entity.AddComponent<SpriteRendererComponent>();
        entity.AddComponent<SelfDestructionScript>(EntityLifeTime);

        if (!name.empty())
            entity.GetComponent<TagComponent>().Tag = name;
    }

    void OnUpdate(Timestep ts) override
    {
        m_TimePassed += ts;

        if (m_TimePassed >= Interval)
        {
            SpawnEntity(fmt::format("Spawned ({0})", m_Counter++));
            m_TimePassed = 0.0f;
        }
    }



    // Assign fields that you expect to be serialized (don't assign pointers or references!)
    BodySpawnerScript& operator=(const BodySpawnerScript& other)
    {
        if (this == &other)
            return *this;

        Interval = other.Interval;
        EntityLifeTime = other.EntityLifeTime;
        m_TimePassed = other.m_TimePassed;
        m_Counter = other.m_Counter;

        return *this;
    }

private:

    float EntityLifeTime = 5.0f;
    float Interval = 5.0f;
    int m_Counter = 0;

    float m_TimePassed = 0.0f;

    SERIALIZE_CLASS(BodySpawnerScript,
        FIELD(Interval)
        FIELD(m_TimePassed)
        FIELD(EntityLifeTime)
        FIELD(m_Counter)
    )
};
