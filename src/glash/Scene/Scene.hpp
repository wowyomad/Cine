#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"
#include "glash/Renderer/EditorCamera.hpp"
#include "glash/Utils/StringUtils.hpp"

#include "Components.hpp"
#include "ComponentSerializer.hpp"

#include <entt/entt.hpp>


namespace Cine
{
	class Entity;
	class NativeScript;

	using ComponentCreater = std::function<void(entt::registry&, entt::entity)>;
	using ComponentRemover = std::function<void(entt::registry&, entt::entity)>;
	using ScriptUpdates = std::function<void(entt::registry&, Timestep)>;

	using SerializerFunc = std::function<YAML::Node(entt::registry&, entt::entity)>;
	using DeserializerFunc = std::function<void(entt::registry&, entt::entity, const YAML::Node&)>;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity GetEntity(const std::string& name);
		void DestroyEntity(Entity entity);

		void SetMainCamera(Entity cameraEntity);
		Entity GetMainCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);

		NativeScript* CreateScriptInstance(const std::string& name);

		auto GetEntities()
		{
			return m_Registry.view<entt::entity>();
		}

		const std::map<std::string, ComponentCreater>& GetRegisteredComponents()
		{
			return m_ComponentCreators;
		}

		template<typename Component>
		void RegisterComponent() {
			std::string componentName = Utils::GetClassTypename<Component>();
			m_ComponentCreators[componentName] = [&](entt::registry& registry, entt::entity entity) -> Component&
				{
					auto& component = registry.emplace<Component>(entity);
					OnRegisteredComponentAdded<Component>(entity, component);
					return component;
				};
			m_ComponentRemovers[componentName] = [componentName](entt::registry& registry, entt::entity entity) -> void
				{
					if constexpr (std::is_base_of<NativeScript, Component>::value)
					{
						auto& nsc = registry.get<NativeScriptComponent>(entity);
						auto it = std::find_if(nsc.Scripts.begin(), nsc.Scripts.end(), [&](auto& script) -> bool
							{
								return script.Name == componentName;
							});
						if (it != nsc.Scripts.end())
						{
							it->RemoveScript(); //Where is it being set???
							nsc.Scripts.erase(it);
						}
					}
					else
					{
						registry.remove<Component>(entity);
					}
				};
			m_SerializationRegistry[componentName] = [&](entt::registry& registry, entt::entity entity) -> YAML::Node
				{
					Component* component = registry.try_get<Component>(entity);
					if (component)
					{
						return Serialize(*component);
					}
					else
					{
						return {};
					}
				};
			m_DeserializationRegistry[componentName] = [componentName](entt::registry& registry, entt::entity entity, const YAML::Node& node)
				{
					Component component;
					Deserialize(component, node);
					registry.emplace_or_replace<Component>(entity, component);
					
					if constexpr (std::is_base_of<NativeScript, Component>::value)
					{
						auto& nsc = registry.get<NativeScriptComponent>(entity);
						auto& Scripts = nsc.Scripts;
						auto it = std::find_if(Scripts.begin(), Scripts.end(), [&](auto& script)
							{
								return script.Name == componentName;
							});
						if (it != Scripts.end())
						{
							it->Instance = nullptr;
						}
					}

				};

			if constexpr (std::is_base_of<NativeScript, Component>::value)
			{
				m_UpdateRegistry[componentName] = [&](entt::registry& registry, Timestep ts) {
					registry.view<Component>().each([&](auto entity, Component& component)
						{
							if (component.Enabled)
							{
								component.OnUpdate(ts);
							}
						});
					};
			}
		}

		void AddComponentByName(Entity entity, const std::string& componentName);
		void RemoveComponentByName(Entity entity, const std::string& componentName);

		YAML::Node SerializeComponentByName(Entity entity, const std::string& componentName);
		void DeserializeComponentByName(Entity entity, const std::string& componentName, const YAML::Node& node);

		template <class Component>
		void OnComponentDestroyed()
		{
			CINE_CORE_TRACE("Removed '{}' from '{}'", Utils::GetClassTypename(Component));
		}

	private:

		template <class Component>
		void OnRegisteredComponentAdded(entt::entity entity, Component& component)
		{
			m_Registry.on_destroy<Component>().connect<&OnRegisteredComponentRemoved<Component>>();

			if constexpr (std::is_same<Component, CameraComponent>::value)
			{
				component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight); //Shoudl it be here?
			}

			else if constexpr (std::is_base_of<NativeScript, Component>::value)
			{
				bool hasNativeScriptComponent = m_Registry.all_of<NativeScriptComponent>(entity);
				if (!hasNativeScriptComponent)
				{
					CINE_CORE_TRACE("Added Native Script Component to {}", static_cast<uint32_t>(entity));
					m_Registry.emplace<NativeScriptComponent>(entity);
				}
				auto& nsc = m_Registry.get<NativeScriptComponent>(entity);
				auto instantiateScript = [&, entity]() -> NativeScript*
					{
						return reinterpret_cast<NativeScript*>(&m_Registry.get<Component>(entity));
					};
				auto removeScript = [&, entity]() -> void
					{
						if (m_Registry.valid(entity) && m_Registry.all_of<Component>(entity))
						{
							m_Registry.remove<Component>(entity);
						}
					};
				nsc.Bind<Component>(instantiateScript, removeScript);
			}
			else
			{
				CINE_CORE_TRACE("Added component '{}' to entity '{}'", Utils::GetClassTypename<Component>(), static_cast<uint32_t>(entity));
			}
		}

		template <class Component>
		static void OnRegisteredComponentRemoved()
		{
			CINE_CORE_WARN("Registered component removed '{}'", Utils::GetClassTypename<Component>());
		}

	private:
		std::map<std::string, ComponentCreater> m_ComponentCreators;
		std::map<std::string, ComponentRemover> m_ComponentRemovers;
		std::map<std::string, ScriptUpdates> m_UpdateRegistry;

		std::map<std::string, SerializerFunc> m_SerializationRegistry;
		std::map<std::string, DeserializerFunc> m_DeserializationRegistry;

		std::vector<entt::entity> m_ToDestroyEntities;
		entt::registry m_Registry;
		Entity* m_MainCamera;

		uint32_t m_ViewportWidth = 1;
		uint32_t m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		void DestroyMarkedEntities();
		void UpdateScripts(Timestep ts);
		void InstantiateScripts();
	};

}