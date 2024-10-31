#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"
#include "glash/Renderer/EditorCamera.hpp"
#include "glash/Utils/StringUtils.hpp"

#include "Components.hpp"
#include "Systems.hpp"
#include "ComponentSerializer.hpp"
#include "ScriptEngine.hpp"

#include <entt/entt.hpp>

class b2World;

namespace Cine
{
	class Entity;
	class NativeScript;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		Entity GetEntity(const std::string& name);
		void DestroyEntity(Entity entity);

		void SetMainCamera(Entity cameraEntity);
		Entity GetMainCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnPhysics2DStart();
		void OnRuntimeStart();
		void OnRuntimeStop();	

		bool IsRuntime() const { return m_IsRuntime; }

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);

		inline entt::registry& GetRegistry() { return m_Registry; }

		auto GetEntities()
		{
			return m_Registry.view<entt::entity>();
		}
		Physics2DSystem& GetPhysics2DSystem()
		{
			return m_PhysicsSystem;
		}

		void Clear();
		void SetUpdateScene(bool update);
		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		inline const std::vector<ComponentData>& GetComponentsData() const
		{
			return m_ScriptEngine.GetComponentsData();
		}

		void AddComponentByName(Entity entity, const std::string& componentName);
		void RemoveComponentByName(Entity entity, const std::string& componentName);

		Entity FindEntityByName(const std::string& name);

		YAML::Node SerializeComponentByName(Entity entity, const std::string& componentName);
		void DeserializeComponentByName(Entity entity, const std::string& componentName, YAML::Node& node);

		template <class Component>
		void OnComponentRemoved(entt::entity entity)
		{
			m_ToDestroyComponentCallbacks.push_back([&, entity]()
				{
					CINE_CORE_WARN("Removing component {0}", Utils::GetClassTypename<Component>());
					CINE_CORE_ASSERT(m_Registry.all_of<Component>(entity), "Entity does not have component {}", typeid(Component).name());
					m_Registry.remove<Component>(entity);
				});
		}

		template <class Component>
		void OnComponentDestroyed()
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
			else
			{
				CINE_CORE_TRACE("Added component '{}' to entity '{}'", Utils::GetClassTypename<Component>(), static_cast<uint32_t>(entity));
			}
		}

	private:
		void DestroyEntities();
		void DestroyComponents();
		void UpdateScripts(Timestep ts);
		void InstantiateScripts();

		using DestroyComponentCallback = std::function<void()>;

	private:
		ScriptEngine& m_ScriptEngine;
		entt::registry m_Registry;
		std::string m_Name;

		std::vector<entt::entity> m_ToDestroyEntities;
		std::vector<DestroyComponentCallback> m_ToDestroyComponentCallbacks;
		bool m_UpdateScene = true;
		bool m_IsRuntime = false;

		Entity* m_MainCamera;

		uint32_t m_ViewportWidth = 1;
		uint32_t m_ViewportHeight = 1;

		Physics2DSystem m_PhysicsSystem;


		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		friend class Physics2DSystem;
	};

}