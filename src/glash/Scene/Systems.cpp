#include "glash/glash_pch.hpp"

#include "Systems.hpp"
#include "Components.hpp"
#include "Entity.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/Renderer2D.hpp"

namespace Cine
{
	static void UpdateWorldTransforms(entt::registry& registry)
	{
		auto view = registry.view<TransformComponent, CachedTransform, HierarchyComponent>();

		std::function<void(entt::entity, const glm::mat4&)> UpdateTransformRecursively =
			[&](auto entity, const glm::mat4& parentTransform)
			{
				auto&& [transform, cachedTransform] = registry.get<TransformComponent, CachedTransform>(entity);

				cachedTransform.CachedTransform = parentTransform * transform.GetLocalTransform();

				auto& hierarchy = registry.get<HierarchyComponent>(entity);
				for (auto child : hierarchy.Children)
				{
					UpdateTransformRecursively(child, cachedTransform.CachedTransform);
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
		UpdateWorldTransforms(registry);

		{
			auto view = registry.view<CachedTransform, SpriteRendererComponent, SpriteSheetComponent, SpriteComponent>();
			for (auto entity : view)
			{
				auto&& [transform, spriteRenderer, spriteSheet, sprite] = view.get<CachedTransform, SpriteRendererComponent, SpriteSheetComponent, SpriteComponent>(entity);
				if (spriteRenderer.UseSprite)
				{
					if (sprite.SpriteIndex >= (int32_t)spriteSheet.Frames.size())
					{
						CINE_CORE_WARN("SpriteFrameIndex ({}) >= spriteSheet.Frames.size ({})", sprite.SpriteIndex, spriteSheet.Frames.size());
						sprite.SpriteIndex = -1;
						continue;
					}
					Renderer2D::DrawSprite(transform.CachedTransform, spriteSheet, sprite.SpriteIndex, sprite.Color);
				}
				else
				{
					Renderer2D::DrawQuad(transform.CachedTransform, sprite.Color);
				}
			}
		}
		{
			auto view = registry.view<CachedTransform, SpriteRendererComponent, SpriteComponent>(entt::exclude<SpriteSheetComponent>);
			for (auto entity : view)
			{
				auto&& [transform, spriteRenderer, sprite] = view.get<CachedTransform, SpriteRendererComponent, SpriteComponent>(entity);
				Renderer2D::DrawQuad(transform.CachedTransform, sprite.Color);
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
}