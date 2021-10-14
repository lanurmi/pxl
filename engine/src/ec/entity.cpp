#include <pxl/ec/entity.h>

using namespace pxl;

u16 Entity::id() const
{
	return _id;
}

World* Entity::world() 
{
	return _world;
}

const World* Entity::world() const
{
	return _world;
}