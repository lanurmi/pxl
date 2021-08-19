#pragma once
#include <pxl/utils/scene.h>

namespace pxl
{
	namespace scenes
	{
		template<class T>
		T* set() {
			auto next = new T();
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