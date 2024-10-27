#include "plugin.hpp"
#include <iostream>
#include <entt/entt.hpp>

#include "GLFW/glfw3.h" //Temporarily


// [INCLUDES]
#include "D:\Dev\Game Development\Cine\Cine Editor\Assets\Plugin\..\Scripts/ColorScript.hpp"
#include "D:\Dev\Game Development\Cine\Cine Editor\Assets\Plugin\..\Scripts/RotatorScript.hpp"

using namespace Cine;

using EmplaceComponentCall = void(*)(entt::entity);
using DestroyComponentCall = void(*)(entt::entity);
using UpdateScriptComponentCall = void(*)(Timestep ts);
using SerializeComponentCall = YAML::Node(*)(entt::entity entity);
using DeserializeComponentCall = void(*)(YAML::Node&, entt::entity);

entt::registry* s_Registry = nullptr;
std::map<std::string, EmplaceComponentCall> Creators;
std::map<std::string, DestroyComponentCall> Destroyers;
std::map<std::string, UpdateScriptComponentCall> Updaters;
std::map<std::string, SerializeComponentCall> Serializers;
std::map<std::string, DeserializeComponentCall> Deserializers;

template <class Component>
void RegisterComponent()
{
	std::string name = Utils::GetClassTypename<Component>();
	Creators[name] = [](entt::entity entity) -> void
		{
			OnComponentAdded<Component>(entity);
		};
	Destroyers[name] = [](entt::entity entity) -> void
		{
			OnComponentRemoved<Component>(entity);
		};
	Serializers[name] = [](entt::entity entity) -> 	YAML::Node
		{
			Component* component = s_Registry->try_get<Component>(entity);
			if (component)
			{
				return Cine::Serialize(*component);
			}
			else
			{
				return {};
			}
		};
	Deserializers[name] = [](YAML::Node& node, entt::entity entity)
		{
			Component component;
			Cine::Deserialize(component, node);
			s_Registry->emplace_or_replace<Component>(entity, component);

			if constexpr (std::is_base_of<NativeScript, Component>::value)
			{
				//Make Instance null to make OnCreate call?
			}

		};
	if constexpr (std::is_base_of<NativeScript, Component>::value)
	{
		Updaters[name] = [](Timestep ts)
			{
				auto view = s_Registry->view<Component>();
				std::vector<entt::entity> scriptEnabled;
				std::copy_if(view.begin(), view.end(), std::back_inserter(scriptEnabled), [&view](entt::entity entity)
					{
						return view.get<Component>(entity).Enabled;
					});
				for (auto entity : scriptEnabled)
				{
					auto& component = view.get<Component>(entity);
					component.OnUpdate(ts);
				}
			};

	}
}

template <class Component>
void OnComponentAdded(entt::entity entity)
{
	auto& component = s_Registry->emplace<Component>(entity);

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

template <class Component>
void OnComponentRemoved(entt::entity entity)
{
	if constexpr (std::is_base_of<NativeScript, Component>::value)
	{
		auto& nsc = s_Registry->get<NativeScriptComponent>(entity);
		std::string componentName = Utils::GetClassTypename<Component>();

		auto it = std::find_if(nsc.Scripts.begin(), nsc.Scripts.end(), [&](auto& script) -> bool
			{
				return script.Name == componentName;
			});
		if (it != nsc.Scripts.end())
		{
			it->RemoveScript(); //Don't!
			nsc.Scripts.erase(it);
		}	
	}
	else
	{
		s_Registry->remove<Component>(entity);
	}
}

void InitializeComponents(entt::registry& registry)
{
	s_Registry = &registry;

	// [REGISTER]
	RegisterComponent<ColorScript>();
	RegisterComponent<RotatorScript>();
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

	Internal::Input::Init();
}

YAML::Node SerializeComponent(entt::entity entity, const std::string& componentName)
{
	auto it = Serializers.find(componentName);
	if (it != Serializers.end())
	{
		return it->second(entity);
	}
	return YAML::Node();
}

void DeserializeComponent(entt::entity entity, const std::string& componentName, YAML::Node& node)
{
	auto it = Deserializers.find(componentName);
	if (it != Deserializers.end())
	{
		return it->second(node, entity);
	}
}
