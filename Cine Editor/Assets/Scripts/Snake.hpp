#pragma once
#include "include/ScriptCore.hpp"
#include "SnakeBodySegment.hpp"
#include <format>

using namespace Cine;

class Snake : public NativeScript
{
    public:
        void OnCreate() override
        {
            std::cout << std::format("I'm snake and I was created!");
        }

        void OnUpdate(Timestep ts) override
        {
            
        }

        // Assign fields that you expect to be serialized (don't assign pointers or references!)
        Snake& operator=(const Snake& other)
        {
            if (this == &other)
                return *this;
            
            NativeScript::operator=(other);
            return *this;
        }

    private:
        SnakeBodySegment m_Segment;


        SERIALIZE_CLASS(Snake,
            FIELD(m_Segment)
        )
};
