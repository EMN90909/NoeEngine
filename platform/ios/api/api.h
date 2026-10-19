// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IOS_API_H
#define IOS_API_H

#if defined(IOS_ENABLED)
extern void godot_ios_plugins_initialize();
extern void godot_ios_plugins_deinitialize();
#endif

void register_ios_api();
void unregister_ios_api();

#endif // IOS_API_H
