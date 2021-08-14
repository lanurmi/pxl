#pragma once
#include <pxl/utils/scene.h>

namespace pxl
{
	namespace scenes
	{
		template<class T>
		T* set(T &&s) {
			auto next = new T();
			*next = s;
			void setNextScenePtr(Scene *);//.cpp
			setNextScenePtr(next);
			return next;
		}
		void update();
		void draw();
		void end();
		Scene* scene();
	}
}