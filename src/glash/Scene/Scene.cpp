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
	static b2BodyType CineRigiBody2DTypeToBox2DType(RigidBody2DComponent::BodyType type)
	{
		switch (type)
		{
		case RigidBody2DComponent::BodyType::Static: return b2BodyType::b2_staticBody;
		case RigidBody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
		case RigidBody2DComponent::BodyType::Dynamic: return b2BodyType::b2_dynamicBody;
		}
	}

	Scene::Scene()
		: m_MainCamera(new Entity()), m_ScriptEngine(ScriptEngine::Get()), m_Name("Unnamed Scene")
	{

	}

	// TODO: Handle save before unload.
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

	void Scene::Clear()
	{
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
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponents<TransformComponent, HierarchyComponent, CachedTransform, NativeScriptComponent>();
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
		m_PhysicsWorld = CreateScope<b2World>(b2Vec2(0.0f, -9.8f));

		auto view = m_Registry.view<RigidBody2DComponent>();

		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.Transform();
			auto& rb = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = CineRigiBody2DTypeToBox2DType(rb.Type);
			bodyDef.position = { transform.Translation.x, transform.Translation.y };
			bodyDef.angle = transform.Rotation.z;
			bodyDef.fixedRotation = rb.FixedRotation;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& collider = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(collider.Size.x * transform.Scale.x, collider.Size.y * transform.Scale.y, { collider.Offset.x, collider.Offset.y }, 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = collider.Density;
				fixtureDef.friction = collider.Friction;
				fixtureDef.restitution = collider.Restitution;
				fixtureDef.restitutionThreshold = collider.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);

				rb.RuntimeBody = body;
			}

		}
	}

	void Scene::OnRuntimeStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnRuntimeStop()
	{
		m_PhysicsWorld = {};
	}


	void Scene::OnUpdateRuntime(Timestep ts)
	{
		if (m_UpdateScene)
		{
			UpdateWorldTransforms(m_Registry);
			InstantiateScripts();

			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				auto view = m_Registry.view<RigidBody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& transform = entity.GetComponent<TransformComponent>();
					auto& rb = entity.GetComponent<RigidBody2DComponent>();

					b2Body* body = (b2Body*)rb.RuntimeBody;

					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
				}
			}

			UpdateScripts(ts);
			SpriteAnimationSystem::Update(m_Registry, ts);

			Renderer2D::Clear();
			if (*m_MainCamera)
			{
				auto&& [cameraComponent, transform] = m_MainCamera->GetComponents<CameraComponent, CachedTransform>();
				Renderer2D::BeginScene(cameraComponent.Camera, transform.CachedMatrix);
				SpriteRendererSystem::Update(m_Registry);
				Renderer2D::EndScene();
			}

			DestroyMarkedEntities();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& editorCamera)
	{
		if (m_UpdateScene)
		{
			UpdateWorldTransforms(m_Registry);
			InstantiateScripts();

			Renderer2D::Clear();
			Renderer2D::BeginScene(editorCamera);
			SpriteRendererSystem::Update(m_Registry);
			Renderer2D::EndScene();

			DestroyMarkedEntities();
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
						script.Instance = script.InstantiateScript();
						script.Instance->m_Entity = Entity(entity, this);
						script.Instance->OnCreate();
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

	void Scene::DestroyMarkedEntities()
	{
		for (auto e : m_ToDestroyEntities)
		{
			auto& hierarchy = m_Registry.get<HierarchyComponent>(e);
			Entity entity = { e, this };

			entity.RemoveParent();
			entity.RemoveChildren();

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

