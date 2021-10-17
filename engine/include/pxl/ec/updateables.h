#pragma once

#include <pxl/utils/action.h>
#include <pxl/ec/world.h>
#include <pxl/ec/component_interface.h>

namespace pxl
{
	class Updateables
	{
	public:
		~Updateables();
		void bind(World* world);
		void unbind();
		void sort();
		const pxl::Vector<IComponent*>& get() const;
	private:
		void tryAddComponent(IComponent* component);
		pxl::Vector<IComponent*> updateables;
		bool _sort = false;
		World* _world;
		ActionBindId bindId;
	};
}
