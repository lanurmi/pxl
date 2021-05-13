#include <pxl/utils/sprite.h>

using namespace pxl;

const Sprite::Animation* Sprite::animation(const string& name) const
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animations[i].name == name)
		{
			return &animations[i];
		}
	}
	return nullptr;
}

int Sprite::indexOfAnimation(const string& name) const
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animations[i].name == name)
		{
			return i;
		}
	}
	return -1;
}