#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"
#include "glash/Renderer/EditorCamera.hpp"
#include "glash/Utils/StringUtils.hpp"

#include "Components.hpp"
#include "ComponentSerializer.hpp"
#include "ScriptEngine.hpp"

#include <entt/entt.hpp>

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
		Entity GetEntity(const std::string& name);
		void DestroyEntity(Entity entity);

		void SetMainCamera(Entity cameraEntity);
		Entity GetMainCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);

		inline entt::registry& GetRegistry() { return m_Registry; }

		auto GetEntities()
		{
			return m_Registry.view<entt::entity>();
		}

		inline const std::vector<ComponentData>& GetComponentsData() const
		{
			return m_ScriptEngine.GetComponentsData();
		}

		void AddComponentByName(Entity entity, const std::string& componentName);
		void RemoveComponentByName(Entity entity, const std::string& componentName);

		YAML::Node SerializeComponentByName(Entity entity, const std::string& componentName);
		void DeserializeComponentByName(Entity entity, const std::string& componentName, YAML::Node& node);

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
		void DestroyMarkedEntities();
		void UpdateScripts(Timestep ts);
		void InstantiateScripts();

	private:
		ScriptEngine& m_ScriptEngine;
		entt::registry m_Registry;

		std::vector<entt::entity> m_ToDestroyEntities;

		Entity* m_MainCamera;

		uint32_t m_ViewportWidth = 1;
		uint32_t m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}