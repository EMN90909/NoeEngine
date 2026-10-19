// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "api.h"

#if defined(IOS_ENABLED)

void register_ios_api() {
	godot_ios_plugins_initialize();
}

void unregister_ios_api() {
	godot_ios_plugins_deinitialize();
}

#else

void register_ios_api() {}
void unregister_ios_api() {}

#endif
