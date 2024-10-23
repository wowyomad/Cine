#include "plugin.hpp"
#include <iostream>
#include <entt/entt.hpp>

/*Should put '#include "Scripts/<Name>"'*/
#include "Scripts/TestScript1.hpp"
#include "Scripts/TestScript2.hpp"

using EmplaceScript = void(*)(entt::entity);
using UpdateScript = void(*)(Timestep ts);
using DestroyScript = void(*)(entt::entity);

entt::registry* s_Registry = nullptr;
std::map<std::string, EmplaceScript> Creators;
std::map<std::string, UpdateScript> Updaters;
std::map<std::string, DestroyScript> Destroyers;

template <class Script>
void RegisterScript()
{
	if constexpr (std::is_base_of<NativeScript, Script>::value)
	{
		std::string name = Utils::GetClassTypename<Script>();
		Creators[name] = [](entt::entity entity)
			{
				s_Registry->emplace<Script>(entity);
			};
		Updaters[name] = [](Timestep ts)
			{
				s_Registry->view<Script>().each([ts](auto entity, auto& script)
					{
						script.OnUpdate(ts);
					});
			};
		Destroyers[name] = [](entt::entity entity)
			{
				s_Registry->remove<TestScript1>(entity);
			};
	}
}

void Initialize(entt::registry& registry)
{
	s_Registry = &registry;

	//Register calls here
	RegisterScript<TestScript1>();
	RegisterScript<TestScript2>();
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

void UpdateScripts(Timestep ts)
{
	for (auto&& [name, update] : Updaters)
	{
		update(ts);
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
