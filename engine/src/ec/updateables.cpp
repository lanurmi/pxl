#include <pxl/ec/updateables.h>

using namespace pxl;


Updateables::~Updateables()
{
	unbind();
}

void Updateables::bind(World* world)
{
	_world = world;
	addBindId = _world->componentAdded.bind(std::bind(&Updateables::tryAddComponent, this, std::placeholders::_1));
	destroyBindId = _world->componentDestroyed.bind(std::bind(&Updateables::tryRemoveComponent, this, std::placeholders::_1));
}

void Updateables::unbind()
{
	if (_world != nullptr)
	{
		_world->componentAdded.unbind(addBindId);
		_world->componentDestroyed.unbind(destroyBindId);
		_world = nullptr;
	}
}

void Updateables::sort()
{
	if (_sort)
	{
		for (auto it: newUpdateables)
		{
			pxl::log::message("added component " + pxl::String(it->typeName()) + " to update list");
			updateables.push_back(it);
		}
		newUpdateables.clear();

		std::sort(updateables.begin(), updateables.end(), [](const IComponent* d0, const IComponent* d1) -> bool {
			return d0->order() < d1->order();
			});

		_sort = false;
	}
}

const pxl::Vector<IComponent*>& Updateables::get() const
{
	return updateables;
}

void Updateables::tryAddComponent(IComponent* component)
{
	if (auto d = dynamic_cast<IUpdateable*>(component) != nullptr)
	{
		newUpdateables.push_back(component);
		_sort = true;
	}
}

void Updateables::tryRemoveComponent(IComponent* component)
{
	if (auto d = dynamic_cast<IUpdateable*>(component) != nullptr)
	{
		// I Dont like this if def, maybe our own vector should work like stl
#ifdef PXL_USE_STL_CONTAINERS
		updateables.erase(std::remove(updateables.begin(), updateables.end(), component), updateables.end());
#else
		v.erase(c);
#endif
	}
}

