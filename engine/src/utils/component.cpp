#include <pxl/utils/component.h>
#include <pxl/utils/scene.h>

using namespace pxl;

u16 pxl::Component::s_type_counter = 0;

Component::Component() : _entity(nullptr), _update_order(0), _draw_order(0), _typeId(0)
{

}

u16 Component::drawOrder() const
{
	return _draw_order;
}
u16 Component::updateOrder() const
{
	return _update_order;
}

void Component::setUpdateOrder(u16 updateOrder)
{
	_update_order = updateOrder;
	entity()->scene()->sortUpdateables();
}

void Component::setDrawOrder(u16 drawOrder)
{
	_draw_order = drawOrder;
	entity()->scene()->sortDrawables();
}

u16 Component::typeId() const
{
	return _typeId;
}

Entity* Component::entity() const
{
	return _entity;
}