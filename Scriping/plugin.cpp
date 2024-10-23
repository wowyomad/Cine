#include "plugin.hpp"
#include <iostream>
#include <entt/entt.hpp>

/*Should put '#include "Scripts/<Name>"'*/
#include "Scripts/ControllerScript.hpp"
#include "Scripts/TestScript.hpp"

using namespace Cine;

using EmplaceComponent = void(*)(entt::entity);
using DestroyComponent = void(*)(entt::entity);
using UpdateScriptComponent = void(*)(Timestep ts);

entt::registry* s_Registry = nullptr;
std::map<std::string, EmplaceComponent> Creators;
std::map<std::string, DestroyComponent> Destroyers;
std::map<std::string, UpdateScriptComponent> Updaters;

template <class Component>
void RegisterComponent()
{
	std::string name = Utils::GetClassTypename<Component>();
	Creators[name] = [](entt::entity entity)
		{
			auto& component = s_Registry->emplace<Component>(entity);
			OnComponentAdded<Component>(entity, component);
		};
	Destroyers[name] = [](entt::entity entity)
		{
			s_Registry->remove<Component>(entity);
		};
	if constexpr (std::is_base_of<NativeScript, Component>::value)
	{
		Updaters[name] = [](Timestep ts)
			{
				auto view = s_Registry->view<Component>();
				for (auto entity : view)
				{
					auto& component = view.get<Component>(entity);
					component.OnUpdate(ts);
				}
			};
		
	}
}

template <class Component>
void OnComponentAdded(entt::entity entity, Component& component)
{
	if constexpr (std::is_base_of<NativeScript, Component>::value)
	{
		bool hasNativeScriptComponent = s_Registry->all_of<NativeScriptComponent>(entity);
		if (!hasNativeScriptComponent)
		{
			s_Registry->emplace<NativeScriptComponent>(entity);
		}
		auto& nsc = s_Registry->get<NativeScriptComponent>(entity);
		auto instantiateScript = [entity]() -> NativeScript*
			{
				return static_cast<Cine::NativeScript*>(&s_Registry->get<Component>(entity));
			};
		auto removeScript = [entity]() -> void
			{
				if (s_Registry->valid(entity) && s_Registry->all_of<Component>(entity))
				{
					s_Registry->remove<Component>(entity);
				}
			};
		nsc.Bind<Component>(instantiateScript, removeScript);
	}
}

void InitializeComponents(entt::registry& registry)
{
	s_Registry = &registry;
	//Register calls here
	RegisterComponent<TestScript>();
	RegisterComponent<ControllerScript>();
}

void CreateComponent(entt::entity entity, const std::string& componentName)
{
	auto it = Creators.find(componentName);
	if (it != Creators.end())
	{
		Creators[componentName](entity);
	}
}

void RemoveComponent(entt::entity entity, const std::string& componentName)
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

ComponentsData GetComponentsData()
{
	ComponentsData data;
	data.Count = Creators.size();
	data.Names = new char* [data.Count];
	data.IsScript = new bool[data.Count];
	size_t i = 0;
	for (auto&& [name, _] : Creators)
	{
		const size_t size = name.size() + 1;
		data.Names[i] = new char[size];
		strncpy(data.Names[i], name.c_str(), size);
		data.IsScript[i] = true;

		++i;
	}
	return data;
}

void InitializeApplicationContext(Cine::Application* application)
{
	Application::Set(application);
	Input::Init();
}