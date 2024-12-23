#pragma once

#include "glash/Core/Timestep.hpp"
#include "glash/Core/Base.hpp"

#include <entt/entt.hpp>
#include <box2d/box2d.h>


namespace Cine
{
	class Entity;

	class Scene;

	void UpdateWorldTransforms(entt::registry& registry);

	class SpriteRendererSystem
	{
	public:
		static void Update(entt::registry& registry);
	};

	class SpriteAnimationSystem
	{
	public:
		static void Update(entt::registry& registry, Timestep deltaTime);
	};

	class Physics2DSystem
	{
	public:
		Physics2DSystem(Scene& scene);
		~Physics2DSystem();

		void Start();
		void Stop();
		void Update(Timestep ts);

		void AddRigidBody(Entity entity);
		void AddCollider(Entity entity);
		
		void UpdateRigidBodyParameters(Entity entity);
		void RemoveRigidBody(Entity entity);

		void SetScene(Scene& scene);
		
	private:
		class ContactListener : public b2ContactListener
		{
			void BeginContact(b2Contact* contact) override;
			void EndContact(b2Contact* contact) override;

		};
	private:
		Scene* m_Scene = nullptr;
		Scope<b2World> m_PhysicsWorld;
		ContactListener m_ContactListener;

	};
}