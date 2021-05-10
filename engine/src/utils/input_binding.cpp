#include <pxl/utils/input_binding.h>
#include <pxl/engine.h>

pxl::KeyBind::KeyBind(pxl::Key key) : _key(key)
{

}

bool pxl::KeyBind::pressed() const
{
	return pxl::keyboard().pressed(_key);
}

bool pxl::KeyBind::down() const
{
	return pxl::keyboard().down(_key);
}

bool pxl::KeyBind::released() const
{
	return pxl::keyboard().released(_key);
}

pxl::ButtonBind::ButtonBind(int controllerIndex, pxl::Button button) : _controller_index(controllerIndex), _button(button)
{

}

bool pxl::ButtonBind::pressed() const
{
	return pxl::gamepads().pressed(_controller_index, _button);
}

bool pxl::ButtonBind::down() const
{
	return pxl::gamepads().down(_controller_index, _button);
}

bool pxl::ButtonBind::released() const
{
	return pxl::gamepads().released(_controller_index, _button);
}

void pxl::ButtonBind::setGamepadIndex(int controllerIndex)
{
	_controller_index = controllerIndex;
}

pxl::InputBinding::InputBinding() : _controller_index(-1), _buffer_timer(0.0f), _buffer_time(0.0f), _pressed(false), _released(false), _down(false)
{

}

pxl::InputBinding& pxl::InputBinding::bind(Key key)
{
	_key_binds.push_back(KeyBind(key));
	return *this;
}

pxl::InputBinding& pxl::InputBinding::bind(Button button)
{
	_button_binds.push_back(ButtonBind(_controller_index, button));
	return *this;
}

void pxl::InputBinding::update()
{
	_pressed = false;
	_down = false;
	_released = false;
	for (auto& key : _key_binds)
	{
		_pressed |= key.pressed();
		_down |= key.down();
	}

	for (auto& btn : _button_binds)
	{
		_pressed |= btn.pressed();
		_down |= btn.down();
	}
	if (_pressed)
	{
		_buffer_timer = _buffer_time;
	}
}

pxl::InputBinding& pxl::InputBinding::setGamepadIndex(int index)
{
	_controller_index = index;
	for (auto& it : _button_binds)
	{
		it.setGamepadIndex(index);
	}
	return *this;
}

pxl::InputBinding& pxl::InputBinding::setInputBuffer(float bufferTime)
{
	_buffer_time = bufferTime;
	return *this;
}

bool pxl::InputBinding::pressed() const
{
	return _pressed;
}

bool pxl::InputBinding::down() const
{
	return _down;
}

bool pxl::InputBinding::released() const
{
	return _released;
}

bool pxl::InputBinding::buffered() const
{
	return _buffer_timer > 0.0f;
}

void pxl::InputBinding::clearBuffer()
{
	_buffer_timer = 0.0f;
}

pxl::InputBindingRef pxl::Bindings::CreateInput()
{
	pxl::log().message("input binding created");
	auto binding = pxl::InputBindingRef(new InputBinding());
	_input_bindings.push_back(binding);
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
}