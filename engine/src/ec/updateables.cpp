#include <pxl/ec/updateables.h>

using namespace pxl;


Updateables::~Updateables()
{
	unbind();
}

void Updateables::bind(World* world)
{
	_world = world;
	bindId = _world->componentAdded.bind(std::bind(&Updateables::tryAddComponent, this, std::placeholders::_1));
}

void Updateables::unbind()
{
	if (_world != nullptr)
	{
		_world->componentAdded.unbind(bindId);
		_world = nullptr;
	}
}

void Updateables::sort()
{
	if (_sort)
	{
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
		updateables.push_back(component);
		_sort = true;
	}
}