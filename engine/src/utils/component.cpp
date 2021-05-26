#include <pxl/utils/component.h>
#include <pxl/utils/scene.h>

using namespace pxl;

u16 pxl::Component::s_type_counter = 0;

Component::Component() : _entity(nullptr), _typeId(0)
{

}

u16 Component::typeId() const
{
	return _typeId;
}

Entity* Component::entity() const
{
	return _entity;
}