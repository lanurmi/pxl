#include <pxl/ec/component_interface.h>

pxl::Vector<pxl::IComponent*> pxl::Updateables::components = pxl::Vector<pxl::IComponent*>();
bool pxl::Updateables::sort = false;

pxl::Vector<pxl::IComponent*> pxl::Drawables::components = pxl::Vector<pxl::IComponent*>();
bool pxl::Drawables::sort = false;