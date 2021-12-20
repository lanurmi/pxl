#pragma once
#include <pxl/utils/action.h>
#include <pxl/ec/world.h>
#include <pxl/ec/component_interface.h>

namespace pxl
{
	class Drawables
	{
	public:
		~Drawables();
		void bind(World* world);
		void unbind();
		void sort();
		const pxl::Vector<IComponent*>& get() const;
	private:
		void tryAddComponent(IComponent* component);
		void tryRemoveComponent(IComponent* component);
		pxl::Vector<IComponent*> drawables;
		bool _sort = false;
		World* _world;
		ActionBindId addBindId;
		ActionBindId destroyBindId;
	};
}

