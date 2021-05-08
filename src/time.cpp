#include <pxl/time.h>

const pxl::u64 pxl::time::ticks_per_second = 1000000;
float pxl::time::delta = 0.0f;
float pxl::time::true_delta = 0.0f;
pxl::u64 pxl::time::ticks = 0;