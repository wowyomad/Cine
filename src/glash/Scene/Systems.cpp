#include "glash/glash_pch.hpp"

#include "Systems.hpp"
#include "Components.hpp"
#include "Entity.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/Renderer2D.hpp"

namespace Cine
{
	void UpdateWorldTransforms(entt::registry& registry)
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
        auto view = registry.view<TransformComponent, SpriteRendererComponent, SpriteComponent>();
        std::vector<entt::entity> opaqueEntities;
        std::vector<entt::entity> transparentEntities;

        for (auto entity : view)
        {
            auto& sprite = view.get<SpriteComponent>(entity);
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
            auto& transformA = view.get<TransformComponent>(a);
            auto& transformB = view.get<TransformComponent>(b);
            return transformA.Translation.z < transformB.Translation.z;
            });

        std::sort(transparentEntities.begin(), transparentEntities.end(), [&](entt::entity a, entt::entity b) {
            auto& transformA = view.get<TransformComponent>(a);
            auto& transformB = view.get<TransformComponent>(b);
            return transformA.Translation.z > transformB.Translation.z;
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
                Renderer2D::DrawSprite(transform.CachedTransform, spriteSheet, sprite.SpriteIndex, sprite.Color);
            }
            else
            {
                Renderer2D::DrawQuad(transform.CachedTransform, sprite.Color);
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
                Renderer2D::DrawSprite(transform.CachedTransform, spriteSheet, sprite.SpriteIndex, sprite.Color);
            }
            else
            {
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