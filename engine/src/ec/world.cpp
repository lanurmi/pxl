#include <pxl/ec/world.h>
#include <pxl/ec/entity.h>
#include <pxl/ec/component.h>

using namespace pxl;

World::World() 
{

}

World::~World() {
}


void World::awake(int bufferEntities)
{
	for (int i = 0; i < bufferEntities; i++)
	{
		entities.push_back(Entity());
	}
}
Entity* World::entity(const Vec2& position)
{
	for (u32 i = 0; i < entities.size(); i++)
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
	entitiesToBeDestroyed.push_back(entity);
	pxl::log::message( pxl::String::format("Added entity %hu destroylist", entity->id()) );

}

void World::update()
{
	for (auto it : entitiesToBeDestroyed)
	{
		for (auto c : it->_components)
		{
			auto list = componentsByType.find(c->id());
			auto& v = list->second;

			// I Dont like this if def, maybe our own vector should work like stl
#ifdef PXL_USE_STL_CONTAINERS
			v.erase( std::remove(v.begin(), v.end(), c), v.end() );
#else
			v.erase(c);
#endif

			auto r = (IResettable*)c;
			r->reset();

			pxl::log::message(pxl::String::format("Component %s destroyed from entity %hu", c->typeName(), it->id()));

			delete c;
		}
		it->reset();
	}
	entitiesToBeDestroyed.clear();
}
