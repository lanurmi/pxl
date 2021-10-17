#include <pxl/ec/drawables.h>

using namespace pxl;


Drawables::~Drawables()
{
	unbind();
}

void Drawables::bind(World* world)
{
	_world = world;
	bindId = _world->componentAdded.bind(std::bind(&Drawables::tryAddComponent, this, std::placeholders::_1));
}

void Drawables::unbind()
{
	if (_world != nullptr)
	{
		_world->componentAdded.unbind(bindId);
		_world = nullptr;
	}
}

void Drawables::sort()
{
	if (_sort)
	{
		std::sort(drawables.begin(), drawables.end(), [](const IComponent* d0, const IComponent* d1) -> bool {
			return d0->layer() < d1->layer();
		});

		_sort = false;
	}
}

const pxl::Vector<IComponent*>& Drawables::get() const
{
	return drawables;
}

void Drawables::tryAddComponent(IComponent* component)
{
	if (auto d = dynamic_cast<IDrawable*>(component) != nullptr)
	{
		drawables.push_back(component);
		_sort = true;
	}
}