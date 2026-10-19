// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef HOSTFXR_RESOLVER_H
#define HOSTFXR_RESOLVER_H

#include "core/string/ustring.h"

namespace godotsharp {
namespace hostfxr_resolver {

bool try_get_path_from_dotnet_root(const String &p_dotnet_root, String &r_out_fxr_path);
bool try_get_path(String &r_out_dotnet_root, String &r_out_fxr_path);

} //namespace hostfxr_resolver
} //namespace godotsharp

#endif // HOSTFXR_RESOLVER_H
