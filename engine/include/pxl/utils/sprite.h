#pragma once
#include <pxl/types.h>
#include <pxl/graphics/Subtexture.h>

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
			string name;
			vector<Frame> frames;
		};

		string name;
		Vec2 origin;
		vector<Animation> animations;

		const Animation* animation(const string &name) const;
		int indexOfAnimation(const string& name) const;
	};
}