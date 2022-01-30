#pragma once
#include <pxl/types.h>
#include <pxl/graphics/Subtexture.h>
#include <pxl/containers/vector.h>

namespace pxl
{
	class Sprite
	{
	public:
		struct Frame
		{
			Subtexture texture;
			float duration;
		};

		struct Animation
		{
			String name;
			Vector<Frame> frames;
			float duration = 0.0f;
		};

		String name;
		Vec2 origin;
		Vector<Animation> animations;

		const Animation* animation(const String &name) const;
		int indexOfAnimation(const String& name) const;
	};
}

