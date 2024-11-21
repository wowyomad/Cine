#include "glash/glash_pch.hpp"

#include "Systems.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Scene.hpp"

#include "glash/Core/Log.hpp"
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

		CINE_CORE_ASSERT(false, "There's no such rigid body type");
		return b2BodyType::b2_staticBody;
	}

	struct FixtureData
	{
		Entity entity;
		FixtureData(Entity entity) : entity(entity) { };
	};

	void UpdateWorldTransforms(entt::registry& registry)
	{
		auto view = registry.view<TransformComponent, CachedTransform, HierarchyComponent>();

		std::function<void(entt::entity, const glm::mat4&)> UpdateTransformRecursively =
			[&](auto entity, const glm::mat4& parentTransform)
			{
				auto&& [transform, cachedTransform] = registry.get<TransformComponent, CachedTransform>(entity);

				cachedTransform.CachedMatrix = parentTransform * transform.GetLocalTransform();

				auto& hierarchy = registry.get<HierarchyComponent>(entity);
				for (auto child : hierarchy.Children)
				{
					UpdateTransformRecursively(child, cachedTransform.CachedMatrix);
				}
			};

		for (auto entity : view)
		{
			auto& hierarchy = view.get<HierarchyComponent>(entity);
			if (!hierarchy.Parent)
			{
				UpdateTransformRecursively(entity, glm::mat4(1.0f));
			}
		}
	}

	void SpriteRendererSystem::Update(entt::registry& registry)
	{
		auto view = registry.view<TransformComponent, SpriteRendererComponent, SpriteComponent, SpriteLayerComponent>();
		std::vector<entt::entity> opaqueEntities;
		std::vector<entt::entity> transparentEntities;

		for (auto entity : view)
		{
			auto& sprite = view.get<SpriteComponent>(entity);
			auto& layer = view.get<SpriteLayerComponent>(entity);
			if (sprite.Color.a >= 1.0f)
			{
				opaqueEntities.push_back(entity);
			}
			else
			{
				transparentEntities.push_back(entity);
			}
		}

		std::sort(opaqueEntities.begin(), opaqueEntities.end(), [&](entt::entity a, entt::entity b) {
			auto& layerA = view.get<SpriteLayerComponent>(a);
			auto& layerB = view.get<SpriteLayerComponent>(b);
			if (layerA.LayerID == layerB.LayerID)
			{
				auto& transformA = view.get<TransformComponent>(a);
				auto& transformB = view.get<TransformComponent>(b);
				return transformA.Translation.z < transformB.Translation.z;
			}
			return layerA.LayerID > layerB.LayerID;
			});

		std::sort(transparentEntities.begin(), transparentEntities.end(), [&](entt::entity a, entt::entity b) {
			auto& layerA = view.get<SpriteLayerComponent>(a);
			auto& layerB = view.get<SpriteLayerComponent>(b);
			if (layerA.LayerID == layerB.LayerID)
			{
				auto& transformA = view.get<TransformComponent>(a);
				auto& transformB = view.get<TransformComponent>(b);
				return transformA.Translation.z > transformB.Translation.z;
			}
			return layerA.LayerID > layerB.LayerID;
			});

		for (auto entity : opaqueEntities)
		{
			auto&& [transform, spriteRenderer, sprite] = registry.get<CachedTransform, SpriteRendererComponent, SpriteComponent>(entity);
			bool useSprite = spriteRenderer.UseSprite;
			bool hasSpriteSheet = registry.all_of<SpriteSheetComponent>(entity);

			if (useSprite && hasSpriteSheet)
			{
				auto& spriteSheet = registry.get<SpriteSheetComponent>(entity);
				if (sprite.SpriteIndex >= static_cast<int32_t>(spriteSheet.Frames.size()))
				{
					CINE_CORE_WARN("SpriteFrameIndex ({}) >= spriteSheet.Frames.size ({})", sprite.SpriteIndex, spriteSheet.Frames.size());
					continue;
				}
				Renderer2D::DrawSprite(transform.CachedMatrix, spriteSheet, sprite.SpriteIndex, static_cast<int32_t>(entity), sprite.Color);
			}
			else
			{
				Renderer2D::DrawQuad(transform.CachedMatrix, sprite.Color, static_cast<int32_t>(entity));
			}
		}

		for (auto entity : transparentEntities)
		{
			auto&& [transform, spriteRenderer, sprite] = registry.get<CachedTransform, SpriteRendererComponent, SpriteComponent>(entity);
			bool useSprite = spriteRenderer.UseSprite;
			bool hasSpriteSheet = registry.all_of<SpriteSheetComponent>(entity);

			if (useSprite && hasSpriteSheet)
			{
				auto& spriteSheet = registry.get<SpriteSheetComponent>(entity);
				if (sprite.SpriteIndex >= static_cast<int32_t>(spriteSheet.Frames.size()))
				{
					CINE_CORE_WARN("SpriteFrameIndex ({}) >= spriteSheet.Frames.size ({})", sprite.SpriteIndex, spriteSheet.Frames.size());
					continue;
				}
				Renderer2D::DrawSprite(transform.CachedMatrix, spriteSheet, sprite.SpriteIndex, static_cast<int32_t>(entity), sprite.Color);
			}
			else
			{
				Renderer2D::DrawQuad(transform.CachedMatrix, sprite.Color, static_cast<int32_t>(entity));
			}
		}
	}


	//TODO: Checks?
	void SpriteAnimationSystem::Update(entt::registry& registry, Timestep deltaTime)
	{
		auto view = registry.view<SpriteComponent, SpriteAnimationComponent>();
		for (auto entity : view)
		{
			auto&& [sc, ac] = view.get<SpriteComponent, SpriteAnimationComponent>(entity);

			if (!ac.State.Play)
			{
				continue;
			}

			if (ac.State.DesiredAnimation != ac.State.CurrentAnimation)
			{
				ac.State.CurrentAnimation = ac.State.DesiredAnimation;
				ac.State.CurrentTime = 0.0f;
				continue;
			}

			if (ac.State.CurrentAnimation < 0 || ac.State.CurrentAnimation >= ac.Animations.size())
			{
				continue;
			}

			const auto& currentAnimation = ac.Animations[ac.State.CurrentAnimation];

			ac.State.CurrentTime += deltaTime;
			float frameDuration = currentAnimation.Duration / currentAnimation.SpriteFrames.size();
			int frameIndex = static_cast<int>(ac.State.CurrentTime / frameDuration);

			if (currentAnimation.Loop)
			{
				frameIndex %= currentAnimation.SpriteFrames.size();
			}
			else
			{
				frameIndex = std::min(frameIndex, static_cast<int>(currentAnimation.SpriteFrames.size()) - 1);
			}

			sc.SpriteIndex = currentAnimation.SpriteFrames[frameIndex];

			if (!currentAnimation.Loop && ac.State.CurrentTime >= currentAnimation.Duration)
			{
				ac.State.DesiredAnimation = ac.State.DefaultAnimation;
			}
		}
	}

	Physics2DSystem::Physics2DSystem(Scene& scene)
		: m_Scene(&scene)
	{

	}

	Physics2DSystem::~Physics2DSystem()
	{

	}

	void Physics2DSystem::Start()
	{
		m_PhysicsWorld = CreateScope<b2World>(b2Vec2(0.0f, -9.8f));
		m_PhysicsWorld->SetContactListener(&m_ContactListener);
		auto& registry = m_Scene->m_Registry;
		auto view = registry.view<RigidBody2DComponent>();
		for (auto e : view)
		{
			AddRigidBody(Entity(e, m_Scene));
		}
	}

	void Physics2DSystem::Stop()
	{
		m_PhysicsWorld.reset();
	}

	void Physics2DSystem::Update(Timestep ts)
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
	
		auto view = m_Scene->m_Registry.view<RigidBody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, m_Scene };
			auto& transform = entity.Transform();
			auto& rb = entity.GetComponent<RigidBody2DComponent>();

			if (rb.RuntimeBody)
			{
				b2Body& body = *static_cast<b2Body*>(rb.RuntimeBody);
				const auto& position = body.GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body.GetAngle();
			}
		}
	}

	void Physics2DSystem::AddRigidBody(Entity entity)
	{
		if (!entity.HasComponent<RigidBody2DComponent>())
		{
			entity.AddComponent<RigidBody2DComponent>();
		}

		if (!m_PhysicsWorld) return;

		auto& transform = entity.Transform();
		
		auto& rb = entity.GetComponent<RigidBody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = CineRigiBody2DTypeToBox2DType(rb.Type);
		bodyDef.position = { transform.Translation.x, transform.Translation.y };
		bodyDef.angle = transform.Rotation.z;
		bodyDef.fixedRotation = rb.FixedRotation;
		bodyDef.gravityScale = rb.GravityScale;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		rb.RuntimeBody = body;

		if (entity.HasComponent<BoxCollider2DComponent>())
			AddCollider(entity);
	}

	void Physics2DSystem::AddCollider(Entity entity)
	{
		auto& transform = entity.Transform();
		auto& rb = entity.GetComponent<RigidBody2DComponent>();
		auto& collider = entity.GetComponent<BoxCollider2DComponent>();

		b2PolygonShape boxShape;
		boxShape.SetAsBox(collider.Size.x * transform.Scale.x - boxShape.m_radius, collider.Size.y * transform.Scale.y - boxShape.m_radius,
			{ collider.Offset.x, collider.Offset.y }, 0.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = collider.Density;
		fixtureDef.friction = collider.Friction;
		fixtureDef.restitution = collider.Restitution;
		fixtureDef.restitutionThreshold = collider.RestitutionThreshold;
		fixtureDef.isSensor = collider.IsTrigger;
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new FixtureData(entity));
		static_cast<b2Body*>(rb.RuntimeBody)->CreateFixture(&fixtureDef);
	}

	void Physics2DSystem::UpdateRigidBodyParameters(Entity entity)
	{
		if (!m_PhysicsWorld || !entity.HasComponent<RigidBody2DComponent>())
			return;

		auto& rb = entity.GetComponent<RigidBody2DComponent>();
		if (!rb.RuntimeBody)
			return;

		b2Body* body = static_cast<b2Body*>(rb.RuntimeBody);

		body->SetType(CineRigiBody2DTypeToBox2DType(rb.Type));
		body->SetFixedRotation(rb.FixedRotation);
		body->SetGravityScale(rb.GravityScale);

		body->SetLinearVelocity({ 0.0f, 0.0f });
		body->SetAngularVelocity(0.0f);

		auto& transform = entity.Transform();
		body->SetTransform({ transform.Translation.x, transform.Translation.y }, transform.Rotation.z);

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& collider = entity.GetComponent<BoxCollider2DComponent>();

			while (b2Fixture* fixture = body->GetFixtureList())
				body->DestroyFixture(fixture);

			b2PolygonShape boxShape;
			boxShape.SetAsBox(collider.Size.x * transform.Scale.x - boxShape.m_radius, collider.Size.y * transform.Scale.y - boxShape.m_radius,
				{ collider.Offset.x, collider.Offset.y }, 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = collider.Density;
			fixtureDef.friction = collider.Friction;
			fixtureDef.restitution = collider.Restitution;
			fixtureDef.restitutionThreshold = collider.RestitutionThreshold;
			fixtureDef.isSensor = collider.IsTrigger;

			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(new FixtureData(entity));

			body->CreateFixture(&fixtureDef);
		}
	}

	void Physics2DSystem::RemoveRigidBody(Entity entity)
	{
		if (!m_PhysicsWorld)
			return;

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			auto& rb = entity.GetComponent<RigidBody2DComponent>();
			if (rb.RuntimeBody)
			{
				b2Body* body = static_cast<b2Body*>(rb.RuntimeBody);

				m_PhysicsWorld->DestroyBody(body);

				rb.RuntimeBody = nullptr;
			}
		}
	}

	void Physics2DSystem::SetScene(Scene& scene)
	{
		m_Scene = &scene;
	}

	void Physics2DSystem::ContactListener::BeginContact(b2Contact* contact)
	{
		auto entityA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetUserData().pointer);
		auto entityB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetUserData().pointer);
		if (entityA && entityB)
		{
			if (contact->GetFixtureA()->IsSensor())
				entityA->OnTriggerEnter(*entityB);
			if (contact->GetFixtureB()->IsSensor())
				entityB->OnTriggerEnter(*entityA);
		}
	}

	void Physics2DSystem::ContactListener::EndContact(b2Contact* contact)
	{
		auto entityA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetUserData().pointer);
		auto entityB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetUserData().pointer);
		if (entityA && entityB)
		{
			if (contact->GetFixtureA()->IsSensor())
				entityA->OnTriggerExit(*entityB);
			if (contact->GetFixtureB()->IsSensor())
				entityB->OnTriggerExit(*entityA);
		}
	}
}