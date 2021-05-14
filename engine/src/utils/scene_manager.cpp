#include <pxl/utils/scene_manager.h>

using namespace pxl;

void SceneManager::update()
{
	if (_next_scene != nullptr)
	{
		if (_current_scene != nullptr)
		{
			_current_scene->end();
		}
		delete _current_scene;
		_current_scene = _next_scene;
		_next_scene = nullptr;
		_current_scene->begin();
	}

	if (_current_scene != nullptr)
	{
		_current_scene->update();
	}
}

void SceneManager::draw()
{
	if (_current_scene != nullptr)
	{
		_current_scene->draw();
	}
}

void SceneManager::end()
{
	if (_current_scene != nullptr)
	{
		_current_scene->end();
	}
	delete _next_scene;
	delete _current_scene;
}

const Scene* SceneManager::scene() const
{
	return _current_scene;
}

Scene* SceneManager::scene()
{
	return _current_scene;
}