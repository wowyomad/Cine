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
		struct ViewportData
		{
			uint32_t Width = 1;
			uint32_t Height = 1;
			uint32_t x = 0;
			uint32_t y = 0;
			bool Focused = false;
			bool Hovered = false;
		};

		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		Entity GetEntity(const std::string& name);
		void DestroyEntity(Entity entity);

		glm::vec3 ScreenToWorldSpace(const glm::vec2& screenSpace);
		glm::vec2 WorldToScreenSpace(const glm::vec3& worldSpace);
		

		void SetMainCamera(Entity cameraEntity);
		Entity GetMainCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);
		void OnViewportPositionChange(uint32_t x, uint32_t y);

		const ViewportData& GetViewportData() { return m_ViewportData; }
		void SetViewportFocused(bool focused) { m_ViewportData.Focused = focused; }
		void SetViewportHovered(bool hovered) { m_ViewportData.Hovered = hovered; }
		bool IsViewportFocused() const { return m_ViewportData.Focused; }
		bool IsViewportHovered() const { return m_ViewportData.Hovered; }
		bool IsViewportHoveredAndFocused() const { return m_ViewportData.Hovered && m_ViewportData.Focused; }


		void OnPhysics2DStart();
		void OnRuntimeStart();
		void OnRuntimePause();
		void OnRuntimeStop();	

		bool IsRuntime() const { return m_IsRuntime; }

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);

		inline entt::registry& GetRegistry() { return m_Registry; }

		Entity GetEntityById(UUID id);

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
					if constexpr (std::is_same<NativeScriptComponent, Component>::value)
					{
						m_Registry.get<NativeScriptComponent>(entity).Clear();
					}

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
				component.Camera.SetViewportSize(m_ViewportData.Width, m_ViewportData.Height); //Shoudl it be here?
			}

			if constexpr (std::is_same<Component, SpriteRendererComponent>::value)
			{
				m_Registry.emplace_or_replace<SpriteComponent>(entity);
				m_Registry.emplace_or_replace<SpriteSheetComponent>(entity);
			}

			if constexpr (std::is_same<Component, RigidBody2DComponent>::value)
			{
				if (!m_Registry.any_of<BoxCollider2DComponent>(entity))
				{
					m_Registry.remove<RigidBody2DComponent>(entity);
				}
			}

			if constexpr (std::is_same<Component, BoxCollider2DComponent>::value)
			{
				m_PhysicsSystem.AddRigidBody(Entity(entity, this));
			}

			if constexpr (std::is_base_of<NativeScript, Component>::value)
			{
				//Temporarily taken from Scripts!
				bool hasNativeScriptComponent = m_Registry.all_of<NativeScriptComponent>(entity);
				if (!hasNativeScriptComponent)
				{
					m_Registry.emplace<NativeScriptComponent>(entity);
				}
				auto& nsc = m_Registry.get<NativeScriptComponent>(entity);
				auto instantiateScript = [&, entity]() -> NativeScript*
					{
						CINE_CORE_INFO("Instantiating Script {0} in {1}", Utils::GetClassTypename<Component>(), (uint32_t)entity);
						return static_cast<Cine::NativeScript*>(&m_Registry.get<Component>(entity));
					};
				auto removeScript = [&, entity]() -> void
					{
						CINE_CORE_WARN("Removing Script {0} from {1}", Utils::GetClassTypename<Component>(), (uint32_t)entity);
						if (m_Registry.valid(entity) && m_Registry.all_of<Component>(entity))
						{
							m_Registry.get<Component>(entity).OnDestroy();
							m_Registry.remove<Component>(entity);
						}
					};
				nsc.Bind<Component>(instantiateScript, removeScript);
				//
			}

			CINE_CORE_TRACE("Added component '{}' to entity '{}'", Utils::GetClassTypename<Component>(), static_cast<uint32_t>(entity));

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
		ViewportData m_ViewportData;

		Entity* m_MainCameraEntity;

		Physics2DSystem m_PhysicsSystem;


		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		friend class Physics2DSystem;
		friend class NativeScript;

	};

}