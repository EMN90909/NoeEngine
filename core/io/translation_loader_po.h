// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TRANSLATION_LOADER_PO_H
#define TRANSLATION_LOADER_PO_H

#include "core/io/file_access.h"
#include "core/io/resource_loader.h"
#include "core/string/translation.h"

class TranslationLoaderPO : public ResourceFormatLoader {
public:
	static Ref<Resource> load_translation(Ref<FileAccess> f, Error *r_error = nullptr);
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;

	TranslationLoaderPO() {}
};

#endif // TRANSLATION_LOADER_PO_H
