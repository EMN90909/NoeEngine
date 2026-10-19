// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SHADER_INCLUDE_H
#define SHADER_INCLUDE_H

#include "core/io/resource.h"
#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/templates/hash_set.h"

class ShaderInclude : public Resource {
	GDCLASS(ShaderInclude, Resource);
	OBJ_SAVE_TYPE(ShaderInclude);

private:
	String code;
	String include_path;
	HashSet<Ref<ShaderInclude>> dependencies;
	void _dependency_changed();

protected:
	static void _bind_methods();

public:
	void set_code(const String &p_text);
	String get_code() const;

	void set_include_path(const String &p_path);
};

class ResourceFormatLoaderShaderInclude : public ResourceFormatLoader {
public:
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
};

class ResourceFormatSaverShaderInclude : public ResourceFormatSaver {
public:
	virtual Error save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags = 0);
	virtual void get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const;
	virtual bool recognize(const Ref<Resource> &p_resource) const;
};

#endif // SHADER_INCLUDE_H
