#pragma once
#include <pxl/types.h>
#include <pxl/backends/keyboard.h>
#include <pxl/backends/gamepad.h>

namespace pxl
{
	class VirtualButton
	{
	public:
		VirtualButton();
		VirtualButton& bind(Key key);
		VirtualButton& bind(Button button);
		VirtualButton& bind(Axis axis);
		VirtualButton& setGamepadIndex(int index);
		VirtualButton& setInputBuffer(float bufferTime);
		VirtualButton& setAxisThreshold(float threshold);

		bool pressed() const;
		bool down() const;
		bool released() const;
		bool buffered() const;
		void clearBuffer();
		void update();
	private:
		std::vector<Key> _key_binds;
		std::vector<Button> _button_binds;
		std::vector<Axis> _axis_binds;
		bool _wait_for_release = true;
		int _controller_index;
		float _buffer_timer;
		float _buffer_time;
		float _axis_threshold = 0.5f;
		bool _pressed;
		bool _released;
		bool _down;
	};

	class VirtualAxis
	{
	public:
		VirtualAxis& bind(Key positiveKey, Key negativeKey);
		VirtualAxis& bind(Button positiveButton, Button negativeButton);
		VirtualAxis& setGamepadIndex(int index);
		void update();
		int sign();
	private:
		VirtualButton _negative;
		VirtualButton _positive;
	};
	using VirtualButtonRef = std::shared_ptr<VirtualButton>;
	using VirtualAxisRef = std::shared_ptr<VirtualAxis>;

	namespace bindings
	{
		VirtualButtonRef createButton();
		VirtualAxisRef createAxis();
		void update();
	};
}