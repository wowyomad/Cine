#pragma once
#include "include/ScriptCore.hpp"

using namespace Cine;

class SnakeBodySegment
{
    public:
        glm::vec3 Position;

    private:
        SERIALIZE_CLASS(SnakeBodySegment,
            FIELD(Position)
        )
};
