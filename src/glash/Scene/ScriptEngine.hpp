#pragma once

#include <entt/entt.hpp>
#include "glash/Utils/PlatformUtils.hpp"
#include "glash/Core/Timestep.hpp"
#include "glash/Core/Application.hpp"

namespace Cine
{
	struct ComponentData
	{
		std::string Name;
		bool IsScript;
	};

	class ScriptEngine
	{
	public:
		struct ComponentsDataC;

		using InitializeApplicationContextCall = void(*)(Application*);
		using InitializeComponentsCall = void(*)(entt::registry&);
		using CreateComponentCall = void(*)(entt::entity, const std::string& scriptName);
		using RemoveComponentCall = void(*)(entt::entity, const std::string& scriptName);
		using UpdateAllScriptsCall = void(*)(Timestep ts);
		using GetComponentsDataCall = ComponentsDataC(*)();

		void LoadLibary(const std::filesystem::path& libraryPath);

		void InitializeComponents(entt::registry& registry);
		void CreateComponent(entt::entity entity, const std::string& componentName);
		void RemoveComponent(entt::entity entity, const std::string& componentName);
		void UpdateScripts(Timestep ts);
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
		};

	private:
		DynamicLibrary m_Library;
		std::vector<ComponentData> m_ComponentsData;
		LibraryScriptCalls m_LibraryCalls;
		
	};
}