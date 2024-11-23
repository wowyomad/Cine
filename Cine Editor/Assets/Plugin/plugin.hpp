#pragma once
#pragma warning(disable : 4190) //disable warning about C++ return type in 'extern "C"'

#define NDEBUG

#include <vector>
#include <string>

#include <entt/entt.hpp>
#include <yaml-cpp/yaml.h>

#include "glash/Cine.hpp"
#include "glash/Scene/Components.hpp"
#include "glash/Core/Application.hpp"
#include "glash/Core/Log.hpp"

#include "input.hpp"
#include "ImguiElement.hpp"

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
    EXPORT void InitializeApplicationContext(Cine::Application*);
    EXPORT void InitializeComponents(entt::registry& registry);
    EXPORT void CreateComponent(entt::entity entity, const std::string& scriptName);
    EXPORT void RemoveComponent(entt::entity entity, const std::string& componentName);
    EXPORT void UpdateScripts(Cine::Timestep ts);
    EXPORT YAML::Node SerializeComponent(entt::entity entity, const std::string& componentName);
    EXPORT void DeserializeComponent(entt::entity entity, const std::string& componentName, YAML::Node& node);
    EXPORT ComponentsData GetComponentsData();

    EXPORT void InitializeInput
    (
        KeyInputFunction isKeyPressedFunc,
        KeyInputFunction isKeyDownFunc,
        KeyInputFunction isKeyUpFunc,
        MouseInputFunction isMousePressedFunc,
        MouseInputFunction isMouseDownFunc,
        MouseInputFunction isMouseUpFunc,
        MousePositionFunction getMousePositoinFunc,
        ToWorldSpaceFunction ToWorldSpaceFunc,
        ToScreenSpaceFunction ToScreenSpaceFunc
    );

    EXPORT void SetActiveRegistry(entt::registry& registry);
    EXPORT void SetImGuiContext(ImGuiContext* context);
    EXPORT void DrawImGui();
    EXPORT void SetLoggers(std::shared_ptr<spdlog::logger>* coreLoggerPtr, std::shared_ptr<spdlog::logger>* clientLoggerPtr);
    EXPORT void OnSceneLoad();
    EXPORT void OnLibraryUnload();
}