#pragma once
#include <pxl/utils/scene.h>

namespace pxl
{
	class SceneManager
	{
	public:
		template<class T>
		T* set(T&& scene);
		void update();
		void draw();
		void end();
		const Scene* scene();
	private:
		Scene* _current_scene;
		Scene* _next_scene;
	};

	template<class T>
	T* SceneManager::set(T&& scene)
	{
		assert(_next_scene == nullptr);
		T* next = new T();
		*next = scene;
		_next_scene = next;
		return next;
	}
}
