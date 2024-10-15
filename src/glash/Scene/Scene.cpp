#include "glash/glash_pch.hpp"
#include "Scene.hpp"

#include "Components.hpp"
#include "Entity.hpp"
#include "ScriptableEntity.hpp"

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
			//make sure that when destroyed, m_MainCamera.m_EntityHandle = entt::null;
		}
		else
		{
			CINE_LOG_ERROR("Tried to assign Camera entity without CameraComponent");
		}
	}

	Entity Scene::GetMainCamera()
	{
		return *m_MainCamera;
	}


	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);
		tag.Tag = !name.empty() ? name : "Unkown Entity";
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);

		//There should be a better way.
		if (entity == *m_MainCamera)
		{
			m_MainCamera->m_EntityHandle = entt::null;
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto& cameraComponent = m_MainCamera->GetComponent<CameraComponent>();

		if (!cameraComponent.FixedAspectRatio)
		{
			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::OnUpdate(Timestep ts)
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (nsc.Instance)
				{
					nsc.Instance->OnUpdate(ts);
				}
				else
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity(entity, this);
					nsc.Instance->OnCreate();
				}
			});

		Renderer2D::Clear();
		if (*m_MainCamera)
		{
			auto&& [cameraComponent, transformComponent] = m_MainCamera->GetComponents<CameraComponent, TransformComponent>();

			Renderer2D::BeginScene(cameraComponent.Camera, transformComponent.GetTransform());

			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

}

