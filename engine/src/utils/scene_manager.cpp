#include <pxl/utils/scene_manager.h>

using namespace pxl;

namespace
{
	Scene* s_current_scene;
	Scene* s_next_scene;
}

void setNextScenePtr(Scene* scene)
{
	assert(s_next_scene == nullptr);
	s_next_scene = scene;
}

void pxl::scenes::update()
{
	if (s_next_scene != nullptr)
	{
		if (s_current_scene != nullptr)
		{
			s_current_scene->end();
		}
		delete s_current_scene;
		s_current_scene = s_next_scene;
		s_next_scene = nullptr;
		s_current_scene->begin();
	}

	if (s_current_scene != nullptr)
	{
		s_current_scene->trueUpdate();
	}
}

void pxl::scenes::draw()
{
	if (s_current_scene != nullptr)
	{
		s_current_scene->draw();
	}
}

void pxl::scenes::end()
{
	if (s_current_scene != nullptr)
	{
		s_current_scene->end();
	}
	delete s_next_scene;
	delete s_current_scene;
}

Scene* pxl::scenes::scene()
{
	return s_current_scene;
}
