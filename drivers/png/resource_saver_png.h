// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RESOURCE_SAVER_PNG_H
#define RESOURCE_SAVER_PNG_H

#include "core/io/image.h"
#include "core/io/resource_saver.h"

class ResourceSaverPNG : public ResourceFormatSaver {
public:
	static Error save_image(const String &p_path, const Ref<Image> &p_img);
	static Vector<uint8_t> save_image_to_buffer(const Ref<Image> &p_img);

	virtual Error save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags = 0);
	virtual bool recognize(const Ref<Resource> &p_resource) const;
	virtual void get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const;

	ResourceSaverPNG();
};

#endif // RESOURCE_SAVER_PNG_H
