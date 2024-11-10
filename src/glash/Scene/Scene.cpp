#include "glash/glash_pch.hpp"
#include "Scene.hpp"

#include "Components.hpp"
#include "Systems.hpp"
#include "Entity.hpp"
#include "NativeScript.hpp"

#include "glash/Utils/PlatformUtils.hpp"

#include "glash/Renderer/Renderer2D.hpp"

#include <box2d/box2d.h>



namespace Cine
{
	Scene::Scene()
		: m_MainCamera(new Entity()),
		m_ScriptEngine(ScriptEngine::Get()),
		m_Name("Unnamed Scene"),
		m_PhysicsSystem(*this)
	{

	}

	// TODO: Handle save before unload.
	Scene::~Scene()
	{

	}

	Entity Scene::GetEntityById(UUID id)
	{
		CINE_CORE_TRACE("Looking for {0}", (uint64_t)id);
		auto view = m_Registry.view<entt::entity>();
		for (auto e : view)
		{
			Entity entity(e, this);
			CINE_CORE_TRACE("Found {0}", (uint64_t)entity.GetID());

			if (id == entity.GetID())
			{
				std::cout << "\n";
				return entity;
			}
		}
		std::cout << "\n";
		return Entity();
	}

	void Scene::SetMainCamera(Entity cameraEntity)
	{
		if (cameraEntity.HasComponent<CameraComponent>())
		{
			*m_MainCamera = cameraEntity;
			m_MainCamera->GetComponent<CameraComponent>().Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
		else
		{
			CINE_LOG_ERROR("Tried to assign Camera entity without CameraComponent");
		}
		//make sure that when destroyed, m_MainCamera.m_EntityHandle = entt::null; Curerntly done primitively right inside DestroyEntity.
	}

	void Scene::Clear()
	{
		m_Registry.clear();
		m_Registry = entt::registry();

		if (m_MainCamera && *m_MainCamera)
		{
			delete m_MainCamera;
			m_MainCamera = new Entity();;
		}
	}
	void Scene::SetUpdateScene(bool update)
	{
		m_UpdateScene = update;
	}



	Entity Scene::GetMainCameraEntity()
	{
		return *m_MainCamera;
	}


	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponents<TransformComponent, HierarchyComponent, CachedTransform, NativeScriptComponent>();
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TagComponent>(!name.empty() ? name : "Unkown Entity");
		return entity;
	}

	Entity Scene::GetEntity(const std::string& name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			if (view.get<TagComponent>(entity).Tag == name)
			{
				return Entity(entity, this);
			}
		}
		return Entity(entt::null, this);
	}

	void Scene::DestroyEntity(Entity entity)
	{
		if (entity && entity.IsValid())
		{
			m_ToDestroyEntities.push_back(entity);

			//There should be a better way.
			if (entity == *m_MainCamera)
			{
				m_MainCamera->m_EntityHandle = entt::null;
			}
		}

	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		if (!*m_MainCamera)
		{
			return;
		}

		auto& cameraComponent = m_MainCamera->GetComponent<CameraComponent>();

		if (!cameraComponent.FixedAspectRatio)
		{
			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnPhysics2DStart()
	{
		m_PhysicsSystem.Start();
	}

	void Scene::OnRuntimeStart()
	{
		m_IsRuntime = true;
		OnPhysics2DStart();
		InstantiateScripts();
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRuntime = false;
		m_PhysicsSystem.Stop();
	}

	void Scene::OnRuntimePause()
	{
		m_PhysicsSystem.Stop();
	}



	void Scene::OnUpdateRuntime(Timestep ts)
	{
		if (m_UpdateScene)
		{
			UpdateWorldTransforms(m_Registry);
			InstantiateScripts();

			m_PhysicsSystem.Update(ts);

			UpdateScripts(ts);
			SpriteAnimationSystem::Update(m_Registry, ts);

			if (*m_MainCamera)
			{
				auto&& [cameraComponent, transform] = m_MainCamera->GetComponents<CameraComponent, CachedTransform>();
				Renderer2D::BeginScene(cameraComponent.Camera, transform.CachedMatrix);
				SpriteRendererSystem::Update(m_Registry);
				Renderer2D::EndScene();
			}

			DestroyComponents();
			DestroyEntities();

		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& editorCamera)
	{
		if (m_UpdateScene)
		{
			UpdateWorldTransforms(m_Registry);
			InstantiateScripts();

			Renderer2D::BeginScene(editorCamera);
			SpriteRendererSystem::Update(m_Registry);
			Renderer2D::EndScene();

			DestroyComponents();
			DestroyEntities();
		}
	}

	void Scene::InstantiateScripts()
	{
		m_Registry.view<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
			{
				for (auto& script : nsc.Scripts)
				{
					if (!script.Instance)
					{
						CINE_CORE_INFO("Instantiating script {0} for {1}", script.Name, (uint32_t)entity);
						Entity e{ entity, this };
						script.Instance = script.InstantiateScript();
						script.Instance->Object = Entity(entity, this);

						try
						{
							script.Instance->OnCreate();
						}
						catch (std::exception& e)
						{
							CINE_CORE_ERROR("Catched exceptoin when Creating script {0}: {1}", script.Name, e.what());
						}
					}
					else if (!script.Instance->Object)
					{
						script.Instance->Object = Entity(entity, this);
					}
				}
			});
	}

	void Scene::UpdateScripts(Timestep ts)
	{
		m_ScriptEngine.UpdateScripts(ts);
	}

	void Scene::AddComponentByName(Entity entity, const std::string& componentName)
	{
		m_ScriptEngine.CreateComponent(entity, componentName);
	}

	void Scene::RemoveComponentByName(Entity entity, const std::string& componentName)
	{
		m_ScriptEngine.RemoveComponent(entity, componentName);
	}

	Entity Scene::FindEntityByName(const std::string& name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			if (view.get<TagComponent>(entity).Tag == name)
			{
				return Entity(entity, this);
			}
		}
		return Entity(); //invalid entity
	}


	YAML::Node Scene::SerializeComponentByName(Entity entity, const std::string& componentName)
	{
		return m_ScriptEngine.SerializeComponent(entity, componentName);
	}

	void Scene::DeserializeComponentByName(Entity entity, const std::string& componentName, YAML::Node& node)
	{
		m_ScriptEngine.DeserializeComponent(entity, node, componentName);
	}

	void Scene::DestroyEntities()
	{
		for (auto e : m_ToDestroyEntities)
		{
			if (e == entt::null || !m_Registry.valid(e))
			{
				continue;
			}

			auto& hierarchy = m_Registry.get<HierarchyComponent>(e);
			Entity entity = { e, this };

			entity.RemoveParent();
			entity.RemoveChildren();

			if (m_Registry.all_of<NativeScriptComponent>(entity))
			{
				auto& nsc = m_Registry.get<NativeScriptComponent>(entity);
				for (auto& script : nsc.Scripts)
				{
					m_ScriptEngine.RemoveComponent(entity, script.Name);
				}
				m_Registry.remove<NativeScriptComponent>(entity);
			}

			if (m_Registry.all_of<RigidBody2DComponent>(e))
			{
				m_PhysicsSystem.RemoveRigidBody(Entity(e, this));
			}
			for (auto [id, storage] : m_Registry.storage()) {
				storage.remove(e);
			}
			m_Registry.destroy(e);
		}
		m_ToDestroyEntities.clear();
	}


	void Scene::DestroyComponents()
	{
		for (auto destroyCallback : m_ToDestroyComponentCallbacks)
		{
			destroyCallback();
		}
		m_ToDestroyComponentCallbacks.clear();
	}



}

