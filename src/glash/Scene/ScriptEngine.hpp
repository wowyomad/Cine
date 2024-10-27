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

	class ScriptEngine
	{
	private:
		using KeyInputFunction = bool(*)(KeyCode);
		using MouseInputFunction = bool(*)(MouseCode);
		using MousePositionFunction = glm::vec2(*)();
		
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
		using InitializeInputCall =  void(*)
		(
			KeyInputFunction,
			KeyInputFunction,
			KeyInputFunction,
			MouseInputFunction,
			MouseInputFunction,
			MouseInputFunction,
			MousePositionFunction
		);

		static ScriptEngine& Get();

		void LoadLibary(const std::filesystem::path& libraryPath);
		void UnloadLibrary();

		void InitializeComponents(entt::registry& registry);
		void CreateComponent(entt::entity entity, const std::string& componentName);
		void RemoveComponent(entt::entity entity, const std::string& componentName);
		void UpdateScripts(Timestep ts);
		YAML::Node SerializeComponent(entt::entity entity, const std::string& componentName);
		void DeserializeComponent(entt::entity entity, YAML::Node& node, const std::string& componentName);
		const std::vector<ComponentData>& GetComponentsData() const;

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
		};

	private:
		DynamicLibrary m_Library;
		std::vector<ComponentData> m_ComponentsData;
		LibraryScriptCalls m_LibraryCalls;

		static ScriptEngine s_ScriptEngine;
	};
	
}