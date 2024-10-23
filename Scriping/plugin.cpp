#include "plugin.hpp"
#include <iostream>
#include <entt/entt.hpp>

using EmplaceScript = void(*)(entt::entity);
using UpdateScript = void(*)();
using DestroyScript = void(*)(entt::entity);

entt::registry* s_Registry = nullptr;
std::map<std::string, EmplaceScript> Creators;
std::map<std::string, UpdateScript> Updaters;
std::map<std::string, DestroyScript> Destroyers;

void Initialize(entt::registry& registry)
{
	s_Registry = &registry;
}

void CreateScript(entt::entity entity, const std::string& componentName)
{
	auto it = Creators.find(componentName);
	if (it != Creators.end())
	{
		Creators[componentName](entity);
	}
}

void RemoveScript(entt::entity entity, const std::string& componentName)
{
	auto it = Creators.find(componentName);
	if (it != Creators.end())
	{
		Destroyers[componentName](entity);
	}
}

void UpdateScripts()
{
	for (auto&& [name, update] : Updaters)
	{
		update();
	}
}

void Destroy()
{

}

ScriptNames* GetScriptNames()
{
	ScriptNames* names = new ScriptNames;
	names->Size = Creators.size();
	names->Names = new char* [Creators.size()];
	size_t i = 0;
	for (auto&& [name, _] : Creators)
	{
		const size_t size = name.size() + 1;
		names->Names[i] = new char[size];
		strncpy(names->Names[i], name.c_str(), size);
		i++;
	}
	return names;
}
