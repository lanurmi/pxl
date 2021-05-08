#include <pxl/config.h>

pxl::Config::Config() : title("Pxl Engine"), width(1920), height(1080), target_framerate(60), fixed_update(false), vertical_sync(true), onBegin(nullptr), onEnd(nullptr), onUpdate(nullptr), onDraw(nullptr)
{
}
