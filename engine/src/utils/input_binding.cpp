#include <pxl/utils/input_binding.h>
#include <pxl/engine.h>

void pxl::VirtualAxis::update()
{
	positive.update();
	negative.update();
}

int pxl::VirtualAxis::sign()
{
	return (positive.down() ? 1 : 0) - (negative.down() ? 1 : 0);
}

pxl::VirtualButton::VirtualButton() : _controller_index(-1), _buffer_timer(0.0f), _buffer_time(0.0f), _pressed(false), _released(false), _down(false)
{

}

pxl::VirtualButton& pxl::VirtualButton::bind(Key key)
{
	_key_binds.push_back(key);
	return *this;
}

pxl::VirtualButton& pxl::VirtualButton::bind(Button button)
{
	_button_binds.push_back(button);
	return *this;
}

void pxl::VirtualButton::update()
{
	_pressed = false;
	_down = false;
	_released = false;
	auto& keyboard = pxl::keyboard();
	for (auto& key : _key_binds)
	{
		_pressed |= keyboard.pressed(key);
		_down |= keyboard.down(key);
	} 

	auto& gamepads = pxl::gamepads();
	for (auto& btn : _button_binds)
	{
		_pressed |= gamepads.pressed(_controller_index, btn);
		_down |= gamepads.down(_controller_index, btn);
	}
	if (_pressed)
	{
		_buffer_timer = _buffer_time;
	}
}

pxl::VirtualButton& pxl::VirtualButton::setGamepadIndex(int index)
{
	_controller_index = index;
	return *this;
}

pxl::VirtualButton& pxl::VirtualButton::setInputBuffer(float bufferTime)
{
	_buffer_time = bufferTime;
	return *this;
}

bool pxl::VirtualButton::pressed() const
{
	return _pressed;
}

bool pxl::VirtualButton::down() const
{
	return _down;
}

bool pxl::VirtualButton::released() const
{
	return _released;
}

bool pxl::VirtualButton::buffered() const
{
	return _buffer_timer > 0.0f;
}

void pxl::VirtualButton::clearBuffer()
{
	_buffer_timer = 0.0f;
}

pxl::VirtualButtonRef pxl::Bindings::CreateInput()
{
	pxl::log().message("button binding created");
	auto binding = pxl::VirtualButtonRef(new VirtualButton());
	_input_bindings.push_back(binding);
	return binding;
}

pxl::VirtualAxisRef pxl::Bindings::CreateAxis()
{
	pxl::log().message("axis binding created");
	auto binding = pxl::VirtualAxisRef(new VirtualAxis());
	_axis_bindings.push_back(binding);
	return binding;
}

void pxl::Bindings::update()
{
	for (int i = _input_bindings.size() - 1; i >= 0; i--)
	{
		if (auto input = _input_bindings[i].lock())
		{
			input->update();
		}
		else
		{
			_input_bindings.erase(_input_bindings.begin() + i);
			pxl::log().message("input binding removed");
		}
	}

	for (int i = _axis_bindings.size() - 1; i >= 0; i--)
	{
		if (auto axis = _axis_bindings[i].lock())
		{
			axis->update();
		}
		else
		{
			_axis_bindings.erase(_axis_bindings.begin() + i);
			pxl::log().message("axisbinding removed");
		}
	}
}