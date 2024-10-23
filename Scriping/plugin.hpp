#pragma once
#include <vector>
#include <string>

#include <entt/entt.hpp>
#include "glash/Cine.hpp"
#include "glash/Scene/Components.hpp"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

struct ComponentsData
{
    char** Names = nullptr;
    bool* IsScript = nullptr;
    size_t Count = 0;
};

extern "C" {
    EXPORT void InitializeComponents(entt::registry& registry);
    EXPORT void CreateComponent(entt::entity entity, const std::string& scriptName);
    EXPORT void RemoveComponent(entt::entity entity, const std::string& componentName);
    EXPORT void UpdateScripts(Cine::Timestep ts);
    EXPORT ComponentsData GetComponentsData();
}