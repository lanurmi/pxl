#include <pxl/utils/component.h>

using namespace pxl;

u16 pxl::Component::s_type_counter = 0;

Component::Component()
{

}

u16 Component::typeId() const
{
	return _type;
}

Entity* Component::entity()
{
	return _entity;
}