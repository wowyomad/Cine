#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"

#include "glash/Renderer/EditorCamera.hpp"

#include "glash/Scene/Components.hpp"

#include "glash/Utils/StringUtils.hpp"

#include <entt/entt.hpp>


namespace Cine
{
	class Entity;
	class NativeScript;

	using ComponentAdder = std::function<void(entt::registry&, entt::entity)>;


	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void SetMainCamera(Entity cameraEntity);
		Entity GetMainCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);

		NativeScript* CreateScriptInstance(const std::string& name);

		template <class T>
		bool IsScriptRegistered()
		{
			std::string name = Utils::GetClassTypename<T>();
			return s_RegisteredScripts.contains(name);
		}

		template<typename Component>
		void RegisterComponent() {
			std::string componentName = Utils::GetClassTypename<Component>();
			s_ComponentRegistry[componentName] = [&](entt::registry& registry, entt::entity entity) {
				auto& component = registry.emplace<Component>(entity);
				OnComponentAdded<Component>(entity, component);
				};
		}

		void AddComponentByName(entt::entity entity, const std::string& componentName) {
			auto it = s_ComponentRegistry.find(componentName);
			if (it != s_ComponentRegistry.end()) {
				it->second(m_Registry, entity);
				std::cout << "Added component: " << componentName << std::endl;
			}
			else {
				std::cout << "Component not found: " << componentName << std::endl;
			}
		}

		template <class T>
		void RegisterScript()
		{
			static_assert(std::is_base_of<NativeScript, T>::value, "T must be a derived class of ScriptableEntity");

			std::string name = Utils::GetClassTypename<T>();
			
			if (s_RegisteredScripts.contains(name))
			{
				CINE_CORE_WARN("Script {} is already registered.", name);
			}

			s_RegisteredScripts[name] = []() -> NativeScript*
				{
					return new T;
				};

			CINE_CORE_TRACE("Registered script {} with type {}", name, typeid(T).name());
		}

	private:

		template <class Component>
		void OnComponentAdded(entt::entity entity, Component& component)
		{
			if constexpr (std::is_same<Component, CameraComponent>::value)
			{
				if (m_Registry.all_of<NativeScriptComponent>(entity))
				{
					CINE_ASSERT(false, "xyu");
				}
				component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
				CINE_CORE_TRACE("Added Camera Component to {}", m_Registry.get<TagComponent>(entity).Tag);
			}

			if constexpr (std::is_base_of<NativeScript, Component>::value)
			{
				bool hasNativeScriptComponent = m_Registry.all_of<NativeScriptComponent>(entity);
				if (!hasNativeScriptComponent)
				{
					CINE_CORE_TRACE("NativeScripteComponent added to {}", static_cast<unsigned int>(entity));
					m_Registry.emplace<NativeScriptComponent>(entity);
				}
				CINE_CORE_TRACE("Script added to {}", static_cast<unsigned int>(entity));
				auto& nsc = m_Registry.get<NativeScriptComponent>(entity);
				nsc.Bind<Component>();
			}
		}

	private:
		static std::unordered_map<std::string, std::function<NativeScript* ()>> s_RegisteredScripts;
		std::unordered_map<std::string, ComponentAdder> s_ComponentRegistry;
		entt::registry m_Registry;
		Entity* m_MainCamera;

		uint32_t m_ViewportWidth = 1;
		uint32_t m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}