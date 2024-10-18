#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"
#include "glash/Renderer/EditorCamera.hpp"
#include "glash/Utils/StringUtils.hpp"
#include "Components.hpp"

#include <entt/entt.hpp>


namespace Cine
{
	class Entity;
	class NativeScript;

	using ComponentAdder = std::function<void(entt::registry&, entt::entity)>;
	using ComponentUpdater = std::function<void(entt::registry&, Timestep)>;

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

		template<typename Component>
		void RegisterComponent() {
			std::string componentName = Utils::GetClassTypename<Component>();
			m_ComponentRegistry[componentName] = [&](entt::registry& registry, entt::entity entity) -> Component&
				{
					auto& component = registry.emplace<Component>(entity);
					OnComponentAdded<Component>(entity, component);
					return component;
				};

			if constexpr (std::is_base_of<NativeScript, Component>::value)
			{
				m_UpdateRegistry[componentName] = [&](entt::registry& registry, Timestep ts) {
					registry.view<Component>().each([&](auto entity, Component& component)
						{
							component.OnUpdate(ts);
						});
					};
			}

		}

		void AddComponentByName(entt::entity entity, const std::string& componentName) {
			auto it = m_ComponentRegistry.find(componentName);
			if (it != m_ComponentRegistry.end()) {
				CINE_CORE_TRACE("Added component '{}' to '{}'", componentName, static_cast<uint32_t>(entity));
				it->second(m_Registry, entity);
			} else
			{
				CINE_CORE_TRACE("Component '{}' not found", componentName);

			}
		}


		template <class Component>
		void OnEntityDestroyed()
		{
			CINE_CORE_TRACE("Removed '{}' from '{}'", Utils::GetClassTypename(Component));
		}

	private:

		template <class Component>
		void OnComponentAdded(entt::entity entity, Component& component)
		{
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
				auto instantiateScript = [&, entity]() -> NativeScript* {
					return reinterpret_cast<NativeScript*>(&m_Registry.get<Component>(entity));
					};
				nsc.Bind<Component>(instantiateScript);
			}
			else
			{
				CINE_CORE_TRACE("Added component '{}' to entity '{}'", Utils::GetClassTypename<Component>(), static_cast<uint32_t>(entity));
			}
		}

	private:
		std::unordered_map<std::string, ComponentAdder> m_ComponentRegistry;
		std::unordered_map<std::string, ComponentUpdater> m_UpdateRegistry;

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