#include "glash/glash_pch.hpp"
#include "Scene.hpp"

#include "Components.hpp"
#include "Entity.hpp"

#include "glash/Renderer/Renderer2D.hpp"

namespace Cine
{
	
	Scene::Scene()
	{

	}


	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);
		tag.Tag = !name.empty() ? name : "Unkown Entity";
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();

		for (auto entity : group)
		{
			auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}

