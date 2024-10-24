#pragma once
#include "Cine.hpp"

using namespace Cine;

class Boba : public NativeScript
{
    public:
        void OnCreate() override
        {
            
        }

        void OnUpdate(Timestep ts) override
        {
            
        }

        // Assign fields that you expect to be serialized (don't assign pointers or references!)
        Boba& operator=(const Boba& other)
        {
            if (this == &other)
                return *this;
            
            return *this;
        }

    private:


        SERIALIZE_CLASS(Boba,

        )
};
