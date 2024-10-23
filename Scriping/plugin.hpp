#pragma once
#include <vector>
#include <string>

#include <entt/entt.hpp>
#include "glash/Cine.hpp"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

struct ScriptNames
{
    char** Names = nullptr;
    size_t Size = 0;
};

extern "C" {
    EXPORT void Initialize(entt::registry& registry);
    EXPORT void CreateScript(entt::entity entity, const std::string& scriptName);
    EXPORT void RemoveScript(entt::entity entity, const std::string& componentName);
    EXPORT void UpdateScripts(Cine::Timestep ts);
    EXPORT ScriptNames* GetScriptNames();
}