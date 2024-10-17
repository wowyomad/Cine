#include "glash/glash_pch.hpp"
#include "Scene.hpp"

#include "Components.hpp"
#include "Systems.hpp"
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
		auto spriteAnimtionView = m_Registry.view<SpriteAnimationComponent, SpriteSheetComponent, SpriteRendererComponent>();
		for (auto entity : spriteAnimtionView)
		{
			auto&& [spriteAnimationComponent, spriteSheetComponent] = spriteAnimtionView.get<SpriteAnimationComponent, SpriteSheetComponent>(entity);
			SpriteAnimationSystem::Update(ts, spriteAnimationComponent, spriteSheetComponent);
			auto&& spriteRenderer = spriteAnimtionView.get<SpriteRendererComponent>(entity);
			spriteRenderer.SpriteSheetIndex = SpriteAnimationSystem::GetCurrentSpriteIndex(spriteAnimationComponent, spriteSheetComponent);
		}

		Renderer2D::Clear();

		Renderer2D::BeginScene(editorCamera);

		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto entity : group)
		{
			auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			bool hasSpriteSheet = m_Registry.all_of<SpriteSheetComponent>(entity);
			if (sprite.UseSprite && hasSpriteSheet)
			{
				auto& spriteSheet = m_Registry.get<SpriteSheetComponent>(entity);
				if (sprite.SpriteSheetIndex >= 0 && sprite.SpriteSheetIndex < spriteSheet.Frames.size())
				{
					Renderer2D::DrawSprite(transform.GetTransform(), spriteSheet, sprite.SpriteSheetIndex, 1.0f, sprite.Color);
				}
				else
				{
					sprite.UseSprite = false;
					sprite.SpriteSheetIndex = 0;
				}
			}
			else
			{
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}
		}

		Renderer2D::EndScene();

	}

	void Scene::OnUpdateRuntime(Timestep ts)
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

