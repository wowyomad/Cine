#pragma once
#include "include/Base.hpp"

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
