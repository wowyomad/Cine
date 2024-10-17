#pragma once

#include "glash/Core/Timestep.hpp"
#include "Components.hpp"

namespace Cine
{
	class SpriteAnimationSystem
	{
	public:
		static void Update(Timestep deltaTime, SpriteAnimationComponent& animationComp, const SpriteSheetComponent& spriteSheetComp)
		{
			//Should never happen
			if (animationComp.Animations.find(animationComp.CurrentAnimation) == animationComp.Animations.end())
			{
				return;
			}

			auto& animation = animationComp.Animations[animationComp.CurrentAnimation];
			animationComp.ElapsedTime += deltaTime;

			if (animationComp.ElapsedTime >= animation->Frames[animationComp.CurrentFrame].Duration)
			{
				animationComp.ElapsedTime = 0.0f;
				animationComp.CurrentFrame++;

				if (animationComp.CurrentFrame >= static_cast<int>(animation->Frames.size()))
				{
					animationComp.CurrentFrame = animation->Loop ? 0 : static_cast<int>(animation->Frames.size() - 1);
				}
			}
		}

		static const Ref<Sprite> GetCurrentSprite(const SpriteAnimationComponent& animationComp, const SpriteSheetComponent& spriteSheetComp) {
			auto it = animationComp.Animations.find(animationComp.CurrentAnimation);
			CINE_CORE_ASSERT(it != animationComp.Animations.end(), "Animation {0} not present", animationComp.CurrentAnimation);

			const auto& animation = animationComp.Animations.at(animationComp.CurrentAnimation);
			int spriteIndex = animation->Frames[animationComp.CurrentFrame].SpriteIndex;
			int maxIndex = static_cast<int>(animation->Frames.size() - 1);
			CINE_CORE_ASSERT(spriteIndex >= 0 && spriteIndex <= maxIndex, "Sprite index {0} out of range 0-{1}", spriteIndex, maxIndex);

			return CreateRef<Sprite>(spriteSheetComp.Texture, spriteSheetComp.Frames[spriteIndex]);
		}
	};
}