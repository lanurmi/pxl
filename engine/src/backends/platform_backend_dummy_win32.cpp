#ifdef PXL_PLATFORM_DUMMY_WIN32

#include <pxl/backends/platform_backend.h>
#include <Windows.h>
#include <assert.h>

void* pxl::platform::glGetFunc(const String& name)
{
	void* p = (void*)wglGetProcAddress(name);
	if (p == 0 ||
		(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
		(p == (void*)-1))   {
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void*)GetProcAddress(module, name);
	}
	return p;
}

void pxl::platform::vsync(bool sync)
{
}

void* pxl::platform::glCreateContext()
{
	return nullptr;
}

void pxl::platform::glBindContext(void *)
{
}

void pxl::platform::glDestroyContext(void *)
{
}


int pxl::platform::cpuCount()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}
#endif