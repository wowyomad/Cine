#pragma once

#include <entt/entt.hpp>
#include "glash/Utils/PlatformUtils.hpp"
#include "glash/Core/Timestep.hpp"
#include "glash/Core/Application.hpp"

#include "yaml-cpp/yaml.h"

namespace Cine
{
	struct ComponentData
	{
		std::string Name;
		bool IsScript;
	};

	class Scene;

	class ScriptEngine
	{
	private:
		using KeyInputFunction = bool(*)(KeyCode);
		using MouseInputFunction = bool(*)(MouseCode);
		using MousePositionFunction = glm::vec2(*)();
		using ToWorldSpaceFunction = glm::vec3(*)(const glm::vec2&);
		using ToScreenSpaceFunction = glm::vec2(const glm::vec3&);
		
	public:
		struct ComponentsDataC;

		using InitializeApplicationContextCall = void(*)(Application*);
		using InitializeComponentsCall = void(*)(entt::registry&);
		using CreateComponentCall = void(*)(entt::entity, const std::string& scriptName);
		using RemoveComponentCall = void(*)(entt::entity, const std::string& scriptName);
		using UpdateAllScriptsCall = void(*)(Timestep ts);
		using SerializeComponentCall = YAML::Node(*)(entt::entity entity, const std::string& componentName);
		using DeserializeComponentCall = void(*)(entt::entity entity, const std::string& componentName, YAML::Node& node);
		using GetComponentsDataCall = ComponentsDataC(*)();
		using SetActiveRegistryCall = void(*)(entt::registry&);
		using SetLoggersCall = void(*)(std::shared_ptr<spdlog::logger>*, std::shared_ptr<spdlog::logger>*);
		using DrawImGuiCall = void(*)();
		using SetImGuiContextCall = void(*)(ImGuiContext*);
		using OnSceneLoadCall = void(*)();
		using OnUnloadCall = void(*)();
		using InitializeInputCall =  void(*)
		(
			KeyInputFunction,
			KeyInputFunction,
			KeyInputFunction,
			MouseInputFunction,
			MouseInputFunction,
			MouseInputFunction,
			MousePositionFunction,
			ToWorldSpaceFunction,
			ToScreenSpaceFunction
		);
		static ScriptEngine& Get();

		void LoadLibary(const std::filesystem::path& libraryPath);
		void UnloadLibrary();
		void OnLibraryUnload();

		void InitializeComponents(entt::registry& registry);
		void CreateComponent(entt::entity entity, const std::string& componentName);
		void RemoveComponent(entt::entity entity, const std::string& componentName);
		void UpdateScripts(Timestep ts);
		YAML::Node SerializeComponent(entt::entity entity, const std::string& componentName);
		void DeserializeComponent(entt::entity entity, YAML::Node& node, const std::string& componentName);
		void SetActiveRegistry(entt::registry& registry);
		const std::vector<ComponentData>& GetComponentsData() const;

		static void SetActiveScene(Scene* scnee);
		static Scene* GetActiveScene();
		void OnSceneLoad();


		void SetImGuiContext(ImGuiContext* context);
		void DrawImGui();

		~ScriptEngine();

	private:
		bool UpdateFunctionCalls();
		void UpdateComponentsData();

		//Must be cleaned on engine side!
		struct ComponentsDataC
		{
			char** Names = nullptr;
			bool* IsScript = nullptr;
			size_t Count = 0;
		};

		struct LibraryScriptCalls
		{
			InitializeApplicationContextCall InitializeApplicationContext;
			InitializeComponentsCall InitializeComponents;
			CreateComponentCall CreateComponent;
			RemoveComponentCall RemoveComponent;
			UpdateAllScriptsCall UpdateScripts;
			GetComponentsDataCall GetComponentsData;
			SerializeComponentCall  SerializeComponent;
			DeserializeComponentCall DeserializeComponent;
			InitializeInputCall InitializeInput;
			SetActiveRegistryCall SetActiveRegistry;
			SetLoggersCall SetLoggers;
			SetImGuiContextCall SetImGuiContext;
			DrawImGuiCall DrawImGui;
			OnSceneLoadCall OnSceneLoad;
			OnUnloadCall OnUnload;

		};

	private:
		DynamicLibrary m_Library;
		std::vector<ComponentData> m_ComponentsData;
		LibraryScriptCalls m_LibraryCalls;
		
		Scene* m_ActiveScene = nullptr;

		static ScriptEngine s_ScriptEngine;

		static bool IsKeyPressedFocused(KeyCode key);
		static bool IsKeyDownFocused(KeyCode key);
		static bool IsKeyUpFocused(KeyCode key);
		static bool IsMouseButtonPressedFocused(MouseCode button);
		static bool IsMouseButtonDownFocused(MouseCode button);
		static bool IsMouseButtonUpFocused(MouseCode button);
		static glm::vec2 GetMouseViewportPosition();
		static glm::vec3 ToWorldSpace(const glm::vec2& screenSpace);
		static glm::vec2 ToScreenSpace(const glm::vec3& worldSpace);
	};
	
}