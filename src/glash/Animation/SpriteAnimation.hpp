#pragma once

#include <vector>

namespace Cine
{
    using AnimationID = int32_t;

    struct Animation {
        AnimationID ID;             
        std::vector<int> FrameIndices;
        float FrameDuration = 0.1f; 
        bool Loop = true;
    };
}