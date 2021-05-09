
#include <pxl/platforms/platform_backend.h>
#include <pxl/time.h>
#include <filesystem>

#include <pxl/engine.h>


#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <assert.h>

static SDL_Window* s_window = nullptr;
static SDL_GameController* s_gamepads[pxl::s_max_gamepads];
static SDL_Haptic* s_gamepad_haptics[pxl::s_max_gamepads];

static char* s_applicationPath = nullptr;
static char* s_userPath = nullptr;

pxl::PlatformBackend::PlatformBackend()
{
}

void pxl::PlatformBackend::init(const pxl::Config& config)
{
	if (SDL_Init(SDL_INIT_VIDEO |SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
	{
		assert(0);
		return;
	}

	int flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

	// sdl Opengl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	s_window = SDL_CreateWindow(config.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width, config.height, flags);
	if (s_window == nullptr)
	{
		assert(0);
		return;
	}
	SDL_SetWindowMinimumSize(s_window, 256, 256);

	s_applicationPath = SDL_GetBasePath();
	s_userPath = SDL_GetPrefPath(nullptr, config.name.c_str());
}

void pxl::PlatformBackend::shutdown()
{
	for (int i = 0; i < pxl::s_max_gamepads; i++)
	{
		SDL_GameControllerClose(s_gamepads[i]);
	}
	for (int i = 0; i < pxl::s_max_gamepads; i++)
	{
		SDL_HapticClose(s_gamepad_haptics[i]);
	}
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
	SDL_free(s_userPath);
	SDL_free(s_applicationPath);
	SDL_DestroyWindow(s_window);
	SDL_Quit();
}

pxl::PlatformBackend::~PlatformBackend()
{

}

static int getGamepadIndex(SDL_JoystickID joystickId)
{
	for (int i = 0; i < pxl::s_max_gamepads; i++)
	{
		if (s_gamepads[i] != nullptr)
		{
			auto p = SDL_GameControllerGetJoystick(s_gamepads[i]);
			if (SDL_JoystickInstanceID(p) == joystickId)
			{
				return i;
			}
		}
	}
	return -1;
}

void pxl::PlatformBackend::update()
{
	int mx, my;
	SDL_GetGlobalMouseState(&mx, &my);
	auto& mouse = pxl::mouse();
	mouse.onMousePosition(pxl::Vec2(mx, my));

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			if (onEnd != nullptr)
			{
				onEnd();
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			MouseButton btn = MouseButton::None;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				btn = MouseButton::Left;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				btn = MouseButton::Right;
			}
			else if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				btn = MouseButton::Middle;
			}
			pxl::mouse().onButtonDown(btn);
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			MouseButton btn = MouseButton::None;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				btn = MouseButton::Left;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				btn = MouseButton::Right;
			}
			else if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				btn = MouseButton::Middle;
			}
			pxl::mouse().onButtonUp(btn);
		}
		else if (event.type == SDL_CONTROLLERDEVICEADDED)
		{
			auto index = event.cdevice.which;
			if (index >= 0 && index < pxl::s_max_gamepads)
			{
				auto ptr = s_gamepads[index] = SDL_GameControllerOpen(index);
				auto name = SDL_GameControllerName(ptr);
				auto vendor = SDL_GameControllerGetVendor(ptr);
				auto product = SDL_GameControllerGetProduct(ptr);
				auto version = SDL_GameControllerGetProductVersion(ptr);

				pxl::gamepads().onConnect(index, pxl::string(name), vendor, product, version);
				int asd = SDL_NumHaptics();
				auto joystick = SDL_GameControllerGetJoystick(ptr);

				auto haptic = SDL_HapticOpen(index);
				if (haptic != nullptr)
				{
					if (SDL_HapticRumbleInit(haptic) == 0)
					{
						s_gamepad_haptics[index] = haptic;
					}
				}
			}
		}
		else if (event.type == SDL_CONTROLLERDEVICEREMOVED)
		{
			auto index = getGamepadIndex(event.cdevice.which);
			if (index >= 0)
			{
				SDL_GameControllerClose(s_gamepads[index]);
				pxl::gamepads().onDisconnect(index);
			}
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.repeat == 0)
			{
				pxl::keyboard().onKeyDown((pxl::Key)event.key.keysym.scancode);
			}
		}
		else if (event.type == SDL_KEYUP)
		{
			if (event.key.repeat == 0)
			{
				pxl::keyboard().onKeyUp((pxl::Key)event.key.keysym.scancode);
			}
		}
		else if (event.type == SDL_CONTROLLERBUTTONDOWN)
		{
			auto index = getGamepadIndex(event.cdevice.which);
			if (index >= 0)
			{
				auto button = (int)pxl::Button::Invalid;
				if (event.cbutton.button >= 0 && event.cbutton.button < pxl::s_max_gamepad_buttons) {
					button = event.cbutton.button;
				}

				pxl::gamepads().onButtonDown(index, button);
			}
		}
		else if (event.type == SDL_CONTROLLERBUTTONUP)
		{
			auto index = getGamepadIndex(event.cdevice.which);
			if (index >= 0)
			{
				auto button = (int)pxl::Button::Invalid;
				if (event.cbutton.button >= 0 && event.cbutton.button < pxl::s_max_gamepad_buttons)
				{
					button = event.cbutton.button;
				}

				pxl::gamepads().onButtonUp(index, button);
			}
		}
		else if (event.type == SDL_CONTROLLERAXISMOTION)
		{
			auto index = getGamepadIndex(event.cdevice.which);
			if (index >= 0)
			{
				int axis = (int)pxl::Axis::Invalid;
				if (event.caxis.axis >= 0 && event.caxis.axis < pxl::s_max_gamepad_axis)
				{
					axis = event.caxis.axis;
				}

				float value;
				if (event.caxis.value >= 0)
				{
					value = event.caxis.value / 32767.0f;
				}
				else
				{
					value = event.caxis.value / 32768.0f;
				}
				pxl::gamepads().onAxis(index, axis, value);
			}
		}
	}
}

void pxl::PlatformBackend::rumble(int index, float time, float strength)
{
	assert(strength >= 0.0f && strength <= 1.0f);
	if (index < pxl::s_max_gamepads)
	{
		auto haptic = s_gamepad_haptics[index];
		SDL_HapticRumblePlay(haptic, strength, time * 1000);
	}
}

pxl::Vec2 pxl::PlatformBackend::position() const
{
	int wx, wy;
	SDL_GetWindowPosition(s_window, &wx, &wy);
	return pxl::Vec2(wx, wy);
}
pxl::string pxl::PlatformBackend::applicationPath() const
{
	return string(s_applicationPath);
}

pxl::string pxl::PlatformBackend::userPath() const
{
	return string(s_userPath);
}

void pxl::PlatformBackend::setTitle(const pxl::string& title)
{
	assert(s_window);
	SDL_SetWindowTitle(s_window, title.c_str());
}

void pxl::PlatformBackend::present()
{
	assert(s_window);
	SDL_GL_SwapWindow(s_window);
}

void pxl::PlatformBackend::sleep(int ms)
{
	assert(ms >= 0);
	SDL_Delay(ms);
}

void pxl::PlatformBackend::vsync(bool vsync)
{
	if (vsync)
	{
		SDL_GL_SetSwapInterval(1);
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}
}

void* pxl::PlatformBackend::glCreateContext()
{
	void* pointer = SDL_GL_CreateContext(s_window);
	if (pointer == nullptr)
	{
		assert(0);
	}
	return pointer;
}

void pxl::PlatformBackend::glBindContext(void* context)
{
	SDL_GL_MakeCurrent(s_window, context);
}

void pxl::PlatformBackend::glDestroyContext(void* context)
{
	SDL_GL_DeleteContext(context);
}

void* pxl::PlatformBackend::glGetFunc(const pxl::string& name)
{
	return SDL_GL_GetProcAddress(name.c_str());
}

pxl::u64 pxl::PlatformBackend::ticks()
{
	auto counter = SDL_GetPerformanceCounter();
	auto per_second = (double)SDL_GetPerformanceFrequency();
	return (u64)(counter * (time::ticks_per_second / per_second));
}

pxl::Vec2 pxl::PlatformBackend::drawSize() const
{
	int w, h;
	SDL_GL_GetDrawableSize(s_window, &w, &h);
	return pxl::Vec2(w, h);
}

pxl::Vec2 pxl::PlatformBackend::size() const
{
	int w, h;
	SDL_GetWindowSize(s_window, &w, &h);
	return pxl::Vec2(w, h);
}

// path

pxl::string pxl::path::extension(const pxl::string& path)
{
	std::filesystem::path filePath(path);
	return filePath.extension().u8string();
}

pxl::string pxl::path::combine(const string lowerPath, const string upperPath)
{
	std::filesystem::path path(lowerPath);
	path.append(upperPath);
	return path.u8string();
}
//file

bool pxl::file::exists(const string& path)
{
	return std::filesystem::is_regular_file(path);
}

// directory

bool pxl::directory::exists(const string& path)
{
	return std::filesystem::is_directory(path);
}

pxl::vector<pxl::string> pxl::directory::files(const string& path, const string& extension)
{
	pxl::vector<pxl::string> result;
	for (const auto& e : std::filesystem::directory_iterator(path))
	{
		auto p = e.path();
		if (extension.empty())
		{
			result.push_back(p.u8string());
		}
		else
		{
			auto ext = p.extension();
			if (ext.u8string() == extension)
			{
				result.push_back(p.u8string());
			}
		}
	}
	return result;
}

