// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "uniform_set_cache_rd.h"

UniformSetCacheRD *UniformSetCacheRD::singleton = nullptr;

void UniformSetCacheRD::_invalidate(Cache *p_cache) {
	if (p_cache->prev) {
		p_cache->prev->next = p_cache->next;
	} else {
		// At beginning of table
		uint32_t table_idx = p_cache->hash % HASH_TABLE_SIZE;
		hash_table[table_idx] = p_cache->next;
	}

	if (p_cache->next) {
		p_cache->next->prev = p_cache->prev;
	}

	cache_allocator.free(p_cache);
	cache_instances_used--;
}
void UniformSetCacheRD::_uniform_set_invalidation_callback(void *p_userdata) {
	singleton->_invalidate(reinterpret_cast<Cache *>(p_userdata));
}

UniformSetCacheRD::UniformSetCacheRD() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

UniformSetCacheRD::~UniformSetCacheRD() {
	if (cache_instances_used > 0) {
		ERR_PRINT("At exit: " + itos(cache_instances_used) + " uniform set cache instance(s) still in use.");
	}
}
