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
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);
		tag.Tag = !name.empty() ? name : "Unkown Entity";
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
		//Scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
			{
				for (auto& script : nsc.Scripts)
				{
					if (script.Instance)
					{
						script.Instance->OnUpdate(ts);
					}
					else
					{
						script.Instance = script.InstantiateScript();
						script.Instance->m_Entity = Entity(entity, this);
						script.Instance->OnCreate();
					}
				}

			});

		//Animation
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

		//Draw
		m_Registry.view<TransformComponent, SpriteRendererComponent>().each([&](auto entity, TransformComponent& transform, SpriteRendererComponent& spriteRenderer)
		{
				bool hasSpriteSheet = m_Registry.all_of<SpriteSheetComponent>(entity);
				if (spriteRenderer.UseSprite && hasSpriteSheet)
				{
					auto& spriteSheet = m_Registry.get<SpriteSheetComponent>(entity);
					if (spriteRenderer.SpriteSheetIndex >= 0 && spriteRenderer.SpriteSheetIndex < spriteSheet.Frames.size())
					{
						Renderer2D::DrawSprite(transform.GetTransform(), spriteSheet, spriteRenderer.SpriteSheetIndex, 1.0f, spriteRenderer.Color);
					}
					else
					{
						spriteRenderer.UseSprite = false;
						spriteRenderer.SpriteSheetIndex = 0;
					}
				}
				else
				{
					Renderer2D::DrawQuad(transform.GetTransform(), spriteRenderer.Color);
				}
		});
		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		m_Registry.view<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& nsc)
			{
				for (auto& script : nsc.Scripts)
				{
					if (script.Instance)
					{
						script.Instance->OnUpdate(ts);
					}
					else
					{
						script.Instance = script.InstantiateScript();
						script.Instance->m_Entity = Entity(entity, this);
						script.Instance->OnCreate();
					}
				}
				
			});

		Renderer2D::Clear();
		if (*m_MainCamera)
		{
			auto&& [cameraComponent, transformComponent] = m_MainCamera->GetComponents<CameraComponent, TransformComponent>();
			Renderer2D::BeginScene(cameraComponent.Camera, transformComponent.GetTransform());

			m_Registry.view<TransformComponent, SpriteRendererComponent>().each([&](auto entity, TransformComponent& transform, SpriteRendererComponent& spriteRenderer)
				{
					bool hasSpriteSheet = m_Registry.all_of<SpriteSheetComponent>(entity);
					if (spriteRenderer.UseSprite && hasSpriteSheet)
					{
						auto& spriteSheet = m_Registry.get<SpriteSheetComponent>(entity);
						if (spriteRenderer.SpriteSheetIndex >= 0 && spriteRenderer.SpriteSheetIndex < spriteSheet.Frames.size())
						{
							Renderer2D::DrawSprite(transform.GetTransform(), spriteSheet, spriteRenderer.SpriteSheetIndex, 1.0f, spriteRenderer.Color);
						}
						else
						{
							spriteRenderer.UseSprite = false;
							spriteRenderer.SpriteSheetIndex = static_cast<int>(spriteSheet.Frames.size()) - 1;
						}
					}
					else
					{
						Renderer2D::DrawQuad(transform.GetTransform(), spriteRenderer.Color);
					}
				});
			Renderer2D::EndScene();

		}
		
		DestroyMarkedEntities();
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

