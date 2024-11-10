#include "scripts_pch.hpp"
#include "plugin.hpp"
#include <iostream>
#include <entt/entt.hpp>

#include "GLFW/glfw3.h" //Temporarily

// [INCLUDES]

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
			Component* serializedComponent = s_Registry->try_get<Component>(entity);
			if (serializedComponent)
			{
				return Cine::Serialize(*serializedComponent);
			}
			else
			{
				return {};
			}
		};
	Deserializers[name] = [](YAML::Node& node, entt::entity entity)
		{
			Component deserializedComponent;
			Cine::Deserialize(deserializedComponent, node);
			s_Registry->emplace_or_replace<Component>(entity, deserializedComponent);

			//TODO: Set Object referece. Object reference is invalid here.

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
					try
					{
						component.OnUpdate(ts);
				
					}
					catch (std::exception& e)
					{
						CINE_CORE_WARN("Catched exception when updating script {0}: {1}", Utils::GetClassTypename<Component>(), e.what());
					}
				}
			};

	}
}

template <class Component>
void OnComponentAdded(entt::entity entity)
{
	if constexpr (std::is_base_of<NativeScript, Component>::value)
	{
		CINE_LOG_INFO("Adding to registry {0} for {1}", Utils::GetClassTypename<Component>(), (uint32_t)entity);
		auto& component = s_Registry->emplace_or_replace<Component>(entity);

		bool hasNativeScriptComponent = s_Registry->all_of<NativeScriptComponent>(entity);
		if (!hasNativeScriptComponent)
		{
			s_Registry->emplace<NativeScriptComponent>(entity);
		}
		auto& nsc = s_Registry->get<NativeScriptComponent>(entity);
		auto instantiateScript = [entity, &component]() -> NativeScript*
			{
				return (NativeScript*)&component;
				//return static_cast<Cine::NativeScript*>(&s_Registry->get<Component>(entity));
			};
		auto removeScript = [entity, &component]() -> void
			{
				if (s_Registry->valid(entity) && s_Registry->all_of<Component>(entity))
				{
					s_Registry->get<Component>(entity).OnDestroy();
					s_Registry->remove<Component>(entity);
				}
			};
		nsc.Bind<Component>(instantiateScript, removeScript);
	}
	else
	{
		std::cout << "Adding Component " << Utils::GetClassTypename<Component>() << " to " << static_cast<uint32_t>(entity) << " in registry " << s_Registry << '\n';
		auto& component = s_Registry->emplace_or_replace<Component>(entity);
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
		data.IsScript[i] = Updaters.find(name) != Updaters.end();

		++i;
	}
	return data;
}

void InitializeInput
(
	KeyInputFunction isKeyPressedFunc,
	KeyInputFunction isKeyDownFunc,
	KeyInputFunction isKeyUpFunc,
	MouseInputFunction isMouseButtonPressedFunc,
	MouseInputFunction isMouseButtonDownFunc,
	MouseInputFunction isMouseButtonUpFunc,
	MousePositionFunction getMousePositionFunc
)
{
	IsKeyPressedFunc = isKeyPressedFunc;
	IsKeyDownFunc = isKeyDownFunc;
	IsKeyUpFunc = isKeyUpFunc;
	IsMouseButtonPressedFunc = isMouseButtonPressedFunc;
	IsMouseButtonDownFunc = isMouseButtonDownFunc;
	IsMouseButtonUpFunc = isMouseButtonUpFunc;
	GetMousePositionFunc = getMousePositionFunc;
}

void SetActiveRegistry(entt::registry& registry)
{
	s_Registry = &registry;
}

void SetLoggers(std::shared_ptr<spdlog::logger>* coreLoggerPtr, std::shared_ptr<spdlog::logger>* clientLoggerPtr)
{
	Log::SetCoreLogger(*coreLoggerPtr);
	Log::SetClientLogger(*clientLoggerPtr);
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
