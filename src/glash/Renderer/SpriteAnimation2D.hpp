#pragma once

namespace Cine
{
	struct SpriteAnimationFrame
	{
		int SpriteIndex;
		float Duration;
	};

	struct SpriteAnimation
	{
		std::string Name;
		std::vector<SpriteAnimationFrame> Frames;
		bool Loop;

		SpriteAnimation(const std::string& name, const std::vector<SpriteAnimationFrame>& frames, bool loop)
			: Name(name), Frames(frames), Loop(loop) {}
	};

}