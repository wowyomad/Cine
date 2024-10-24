#pragma once
#include "Cine.hpp"

using namespace Cine;

class RandomMover : public NativeScript
{
    public:
        void OnCreate() override
        {
            m_Timer.Start();
        }

        void OnUpdate(Timestep ts) override
        {
            m_Timer.OnUpdate(ts);
            m_Time = m_Timer.GetElapsed();

            if(m_Time >= m_Period)
            {
                m_Timer = Timer();
                m_Timer.Start();
                int distance = rand() % 10 + 1 - 5;
                GetComponent<TransformComponent>().Translation.x += distance;
            }
        }

        RandomMover& operator=(const RandomMover& other)
        {
            m_Time = other.m_Time;
            return *this;
        }

    private:
        float m_Time = 0.0f;
        float m_Period = 5.0f;
        Timer m_Timer;
        SERIALIZE_CLASS(RandomMover,
            FIELD(m_Time)
        )
};
