// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEST_INPUT_EVENT_MOUSE_H
#define TEST_INPUT_EVENT_MOUSE_H

#include "core/input/input_event.h"
#include "tests/test_macros.h"

namespace TestInputEventMouse {

TEST_CASE("[InputEventMouse] Mouse button mask is set correctly") {
	InputEventMouse mousekey;

	mousekey.set_button_mask(MouseButtonMask::LEFT);
	CHECK(mousekey.get_button_mask().has_flag(MouseButtonMask::LEFT));

	mousekey.set_button_mask(MouseButtonMask::MB_XBUTTON1);
	CHECK(mousekey.get_button_mask().has_flag(MouseButtonMask::MB_XBUTTON1));

	mousekey.set_button_mask(MouseButtonMask::MB_XBUTTON2);
	CHECK(mousekey.get_button_mask().has_flag(MouseButtonMask::MB_XBUTTON2));

	mousekey.set_button_mask(MouseButtonMask::MIDDLE);
	CHECK(mousekey.get_button_mask().has_flag(MouseButtonMask::MIDDLE));

	mousekey.set_button_mask(MouseButtonMask::RIGHT);
	CHECK(mousekey.get_button_mask().has_flag(MouseButtonMask::RIGHT));
}

TEST_CASE("[InputEventMouse] Setting the mouse position works correctly") {
	InputEventMouse mousekey;

	mousekey.set_position(Vector2{ 10, 10 });
	CHECK(mousekey.get_position() == Vector2{ 10, 10 });

	mousekey.set_position(Vector2{ -1, -1 });
	CHECK(mousekey.get_position() == Vector2{ -1, -1 });
}

TEST_CASE("[InputEventMouse] Setting the global mouse position works correctly") {
	InputEventMouse mousekey;

	mousekey.set_global_position(Vector2{ 10, 10 });
	CHECK(mousekey.get_global_position() == Vector2{ 10, 10 });
	CHECK(mousekey.get_global_position() != Vector2{ 1, 1 });

	mousekey.set_global_position(Vector2{ -1, -1 });
	CHECK(mousekey.get_global_position() == Vector2{ -1, -1 });
	CHECK(mousekey.get_global_position() != Vector2{ 1, 1 });
}
} // namespace TestInputEventMouse

#endif // TEST_INPUT_EVENT_MOUSE_H
