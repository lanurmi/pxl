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

Mat3x2 Entity::matrix() const
{
	if (parent != nullptr)
	{
		return transform.matrix() * parent->matrix();
	}
	else
	{
		return transform.matrix();
	}
}


Vec2 Entity::position() const
{
	if (parent != nullptr)
	{
		auto mat = parent->matrix();
		return pxl::Vec2::transform(transform.position, mat);
	}
	else
	{
		return transform.position;
	}
}

Vec2 Entity::scale() const
{
	if (parent != nullptr)
	{
		auto s = parent->scale();
		return s * transform.scale;
	}
	else
	{
		return transform.scale;
	}
}

float Entity::rotation() const
{
	if (parent != nullptr)
	{
		auto r = parent->rotation();
		return r + transform.rotation;
	}
	else
	{
		return transform.rotation;
	}
}
