// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "mono_gc_handle.h"

#include "mono_gd/gd_mono.h"
#include "mono_gd/gd_mono_cache.h"

void MonoGCHandleData::release() {
#ifdef DEBUG_ENABLED
	CRASH_COND(handle.value && GDMono::get_singleton() == nullptr);
#endif

	if (handle.value && GDMonoCache::godot_api_cache_updated &&
			GDMono::get_singleton()->is_runtime_initialized()) {
		free_gchandle(handle);
		handle.value = nullptr;
	}
}
void MonoGCHandleData::free_gchandle(GCHandleIntPtr p_gchandle) {
	CRASH_COND(!GDMonoCache::godot_api_cache_updated);
	GDMonoCache::managed_callbacks.GCHandleBridge_FreeGCHandle(p_gchandle);
}
