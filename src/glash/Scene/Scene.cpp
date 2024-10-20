#include "glash/glash_pch.hpp"
#include "Scene.hpp"

#include "Components.hpp"
#include "Systems.hpp"
#include "Entity.hpp"
#include "NativeScript.hpp"

#include "glash/Renderer/Renderer2D.hpp"

namespace Cine
{
	Scene::Scene()
		: m_MainCamera(new Entity())
	{

	}

	Scene::~Scene()
	{
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

	Entity Scene::GetMainCameraEntity()
	{
		return *m_MainCamera;
	}


	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponents<TransformComponent, HierarchyComponent, CachedTransform>();
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
		m_ToDestroyEntities.push_back(entity);

		//There should be a better way.
		if (entity == *m_MainCamera)
		{
			m_MainCamera->m_EntityHandle = entt::null;
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (!*m_MainCamera)
		{
			return;
		}

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto& cameraComponent = m_MainCamera->GetComponent<CameraComponent>();

		if (!cameraComponent.FixedAspectRatio)
		{
			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& editorCamera)
	{
		InstantiateScripts();
		UpdateScripts(ts);

		Renderer2D::Clear();
		Renderer2D::BeginScene(editorCamera);
		SpriteRendererSystem::Update(m_Registry);
		SpriteAnimationSystem::Update(m_Registry, ts);
		Renderer2D::EndScene();

		DestroyMarkedEntities();
	}


void Scene::OnUpdateRuntime(Timestep ts)
{
	InstantiateScripts();
	UpdateScripts(ts);

	Renderer2D::Clear();
	if (*m_MainCamera)
	{
		auto&& [cameraComponent, transformComponent] = m_MainCamera->GetComponents<CameraComponent, TransformComponent>();
		Renderer2D::BeginScene(cameraComponent.Camera, transformComponent.GetLocalTransform());
		SpriteRendererSystem::Update(m_Registry);
		SpriteAnimationSystem::Update(m_Registry, ts);
		Renderer2D::EndScene();
	}

	DestroyMarkedEntities();
}

void Scene::InstantiateScripts()
{
	m_Registry.view<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
		{
			for (auto& script : nsc.Scripts)
			{
				if (!script.Instance)
				{
					script.Instance = script.InstantiateScript();
					script.Instance->m_Entity = Entity(entity, this);
					script.Instance->OnCreate();
				}
			}
		});
}

void Scene::UpdateScripts(Timestep ts)
{
	for (auto& [componentName, updateCall] : m_UpdateRegistry)
	{
		updateCall(m_Registry, ts);
	}
}

void Scene::DestroyMarkedEntities()
{
	for (auto entity : m_ToDestroyEntities)
	{
		if (m_Registry.all_of<NativeScriptComponent>(entity))
		{
			auto& nsc = m_Registry.get<NativeScriptComponent>(entity);
			for (auto& script : nsc.Scripts)
			{
				script.Instance->OnDestroy();
			}
		}

		m_Registry.destroy(entity);
	}
	m_ToDestroyEntities.clear();
}



}

