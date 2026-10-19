// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef JOYPAD_UWP_H
#define JOYPAD_UWP_H

#include "core/input/input.h"

ref class JoypadUWP sealed {
	/** clang-format breaks this, it does not understand this token. */
	/* clang-format off */
internal:
	void register_events();
	void process_controllers();
	/* clang-format on */

	JoypadUWP();
	JoypadUWP(InputDefault *p_input);

private:
	enum {
		MAX_CONTROLLERS = 4,
	};

	enum ControllerType {
		GAMEPAD_CONTROLLER,
		ARCADE_STICK_CONTROLLER,
		RACING_WHEEL_CONTROLLER,
	};

	struct ControllerDevice {
		Windows::Gaming::Input::IGameController ^ controller_reference;

		int id = -1;
		bool connected = false;
		ControllerType type = ControllerType::GAMEPAD_CONTROLLER;
		float ff_timestamp = 0;
		float ff_end_timestamp = 0;
		bool vibrating = false;
	};

	ControllerDevice controllers[MAX_CONTROLLERS];

	InputDefault *input = nullptr;

	void OnGamepadAdded(Platform::Object ^ sender, Windows::Gaming::Input::Gamepad ^ value);
	void OnGamepadRemoved(Platform::Object ^ sender, Windows::Gaming::Input::Gamepad ^ value);

	float axis_correct(double p_val, bool p_negate = false, bool p_trigger = false) const;
	void joypad_vibration_start(int p_device, float p_weak_magnitude, float p_strong_magnitude, float p_duration, uint64_t p_timestamp);
	void joypad_vibration_stop(int p_device, uint64_t p_timestamp);
};

#endif // JOYPAD_UWP_H
