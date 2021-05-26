#pragma once
#include <pxl/types.h>
#include <pxl/input/keyboard_keys.h>
#include <pxl/input/gamepad_buttons.h>

namespace pxl
{

	class VirtualButton
	{
	public:
		VirtualButton();
		VirtualButton& bind(Key key);
		VirtualButton& bind(Button button);
		VirtualButton& setGamepadIndex(int index);
		VirtualButton& setInputBuffer(float bufferTime);
		bool pressed() const;
		bool down() const;
		bool released() const;
		bool buffered() const;
		void clearBuffer();
		void update();
	private:
		std::vector<Key> _key_binds;
		std::vector<Button> _button_binds;
		int _controller_index;
		float _buffer_timer;
		float _buffer_time;
		bool _pressed;
		bool _released;
		bool _down;
	};


	class VirtualAxis
	{
	public:
		VirtualButton negative;
		VirtualButton positive;
		void update();
		int sign();
	};
	using VirtualButtonRef = std::shared_ptr<VirtualButton>;
	using VirtualAxisRef = std::shared_ptr<VirtualAxis>;

	class Bindings
	{
	public:
		VirtualButtonRef CreateButton();
		VirtualAxisRef CreateAxis();
		void update();
	private:
		std::vector<std::weak_ptr<VirtualButton>> _input_bindings;
		std::vector<std::weak_ptr<VirtualAxis>> _axis_bindings;
	};
}