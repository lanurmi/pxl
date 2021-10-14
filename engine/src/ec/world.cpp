#include <pxl/ec/world.h>
#include <pxl/ec/entity.h>
#include <pxl/ec/component.h>

using namespace pxl;

u16 ComponentId::idCounter = 0;


World::World() 
{
	for (int i = 0; i < s_max_entities; i++)
	{
		entities.add(Entity());
	}
}

World::~World() {
}


Entity* World::entity(const Vec2& position)
{
	for (u16 i = 0; i < s_max_entities; i++)
	{
		if (entities[i].id() == s_unused_entity)
		{
			entities[i].transform.position = position;
			entities[i]._id = i+1;
			entities[i]._world = this;
			return &entities[i];
		}
	}
	return nullptr;
}

void World::destroy(Entity* entity)
{

	for (auto c : entity->_components)
	{
		c->destroy();
	}
	entitiesToBeDestroyed.add(entity);

}

void World::update()
{
	for (auto it : entitiesToBeDestroyed)
	{
		for (auto c : it->_components)
		{
			auto list = componentsByType.find(c->typeId());
			list->second.erase(c);
			auto r = (IResettable*)c;
			r->reset();
			delete c;
		}
		it->reset();
	}
	entitiesToBeDestroyed.clear();
}
