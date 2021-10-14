#include <pxl/utils/input_binding.h>
#include <pxl/engine.h>
#include <pxl/time.h>
#include <pxl/math/calc.h>

using namespace pxl;


namespace
{
	std::vector<std::weak_ptr<VirtualButton>> s_input_bindings;
	std::vector<std::weak_ptr<VirtualAxis>> s_axis_bindings;
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
	bool wasDown = _down;
	_pressed = false;
	_down = false;
	_released = false;
	for (auto& key : _key_binds)
	{
		_down |= pxl::keyboard::down(key);
	} 

	for (auto& btn : _button_binds)
	{
		_down |= pxl::gamepad::down(_controller_index, btn);
	}
	_pressed = wasDown == false && _down == true;
	_released = wasDown == true && _down == false;

	_buffer_timer = pxl::calc::approach(_buffer_timer, 0.0f, pxl::time::delta);
	if (_pressed)
	{
		_buffer_timer = _buffer_time;
	}
	if (_released)
	{
		_buffer_timer = 0.0f;
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


VirtualAxis& VirtualAxis::bind(Key positiveKey, Key negativeKey) {
	_positive.bind(positiveKey);
	_negative.bind(negativeKey);
	return *this;
}

VirtualAxis& VirtualAxis::bind(Button positiveButton, Button negativeButton) {
	_positive.bind(positiveButton);
	_negative.bind(negativeButton);
	return *this;
}

VirtualAxis& VirtualAxis::setGamepadIndex(int index) {
	_positive.setGamepadIndex(index);
	_negative.setGamepadIndex(index);
	return *this;
}

void pxl::VirtualAxis::update() {
	_positive.update();
	_negative.update();
}

int pxl::VirtualAxis::sign() {
	return (_positive.down() ? 1 : 0) - (_negative.down() ? 1 : 0);
}

pxl::VirtualButtonRef pxl::bindings::createButton()
{
	pxl::log::message("button binding created");
	auto binding = pxl::VirtualButtonRef(new VirtualButton());
	s_input_bindings.push_back(binding);
	return binding;
}

pxl::VirtualAxisRef pxl::bindings::createAxis()
{
	pxl::log::message("axis binding created");
	auto binding = pxl::VirtualAxisRef(new VirtualAxis());
	s_axis_bindings.push_back(binding);
	return binding;
}

void pxl::bindings::update()
{
	for (int i = s_input_bindings.size() - 1; i >= 0; i--)
	{
		if (auto input = s_input_bindings[i].lock())
		{
			input->update();
		}
		else
		{
			s_input_bindings.erase(s_input_bindings.begin() + i);
			pxl::log::message("input binding removed");
		}
	}

	for (int i = s_axis_bindings.size() - 1; i >= 0; i--)
	{
		if (auto axis = s_axis_bindings[i].lock())
		{
			axis->update();
		}
		else
		{
			s_axis_bindings.erase(s_axis_bindings.begin() + i);
			pxl::log::message("axisbinding removed");
		}
	}
}