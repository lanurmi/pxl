#pragma once
#include <pxl/types.h>
#include <pxl/input/keyboard_keys.h>
#include <pxl/input/gamepad_buttons.h>

namespace pxl
{
	class KeyBind
	{
	public:
		KeyBind(Key key);
		bool pressed() const;
		bool down() const;
		bool released() const;
	private:
		Key _key;
	};

	class ButtonBind
	{
	public:
		ButtonBind(int controllerIndex, Button button);
		bool pressed() const;
		bool down() const;
		bool released() const;
		void setGamepadIndex(int controllerIndex);
	private:
		int _controller_index;
		Button _button;	
	};

	class InputBinding
	{
	public:
		InputBinding();
		InputBinding& bind(Key key);
		InputBinding& bind(Button button);
		InputBinding& setGamepadIndex(int index);
		InputBinding& setInputBuffer(float bufferTime);
		bool pressed() const;
		bool down() const;
		bool released() const;
		bool buffered() const;
		void clearBuffer();
		void update();
	private:
		std::vector<KeyBind> _key_binds;
		std::vector<ButtonBind> _button_binds;
		int _controller_index;
		float _buffer_timer;
		float _buffer_time;
		bool _pressed;
		bool _released;
		bool _down;
	};

	using InputBindingRef = std::shared_ptr<InputBinding>;

	class Bindings
	{
	public:
		InputBindingRef CreateInput();
		void update();
	private:
		std::vector<std::weak_ptr<InputBinding>> _input_bindings;
	};
}