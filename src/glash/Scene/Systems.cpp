#include "glash/glash_pch.hpp"

#include "Systems.hpp"
#include "Components.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/Renderer2D.hpp"

namespace Cine
{
	void SpriteRendererSystem::Update(entt::registry& registry)
	{
		{
			auto view = registry.view<TransformComponent, SpriteRendererComponent, SpriteSheetComponent, SpriteComponent>();
			for (auto entity : view)
			{
				auto&& [transform, spriteRenderer, spriteSheet, sprite] = view.get<TransformComponent, SpriteRendererComponent, SpriteSheetComponent, SpriteComponent>(entity);
				if (spriteRenderer.UseSprite)
				{
					if (sprite.SpriteFrameIndex >= (int32_t)spriteSheet.Frames.size())
					{
						CINE_CORE_WARN("SpriteFrameIndex ({}) >= spriteSheet.Frames.size ({})", sprite.SpriteFrameIndex, spriteSheet.Frames.size());
						sprite.SpriteFrameIndex = -1;
						continue;
					}
					Renderer2D::DrawSprite(transform.GetTransform(), spriteSheet, sprite.SpriteFrameIndex, spriteRenderer.Color);
				}
				else
				{
					Renderer2D::DrawQuad(transform.GetTransform(), spriteRenderer.Color);
				}
			}
		}
		{
			auto view = registry.view<TransformComponent, SpriteRendererComponent>(entt::exclude<SpriteSheetComponent, SpriteComponent>);
			for (auto entity : view)
			{
				auto&& [transform, spriteRenderer] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), spriteRenderer.Color);
			}
		}
	}
}