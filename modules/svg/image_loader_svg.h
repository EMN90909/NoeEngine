// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IMAGE_LOADER_SVG_H
#define IMAGE_LOADER_SVG_H

#include "core/io/image_loader.h"

class ImageLoaderSVG : public ImageFormatLoader {
	static HashMap<Color, Color> forced_color_map;

	void _replace_color_property(const HashMap<Color, Color> &p_color_map, const String &p_prefix, String &r_string);

public:
	static void set_forced_color_map(const HashMap<Color, Color> &p_color_map);

	Error create_image_from_utf8_buffer(Ref<Image> p_image, const PackedByteArray &p_buffer, float p_scale, bool p_upsample);
	Error create_image_from_string(Ref<Image> p_image, String p_string, float p_scale, bool p_upsample, const HashMap<Color, Color> &p_color_map);

	virtual Error load_image(Ref<Image> p_image, Ref<FileAccess> p_fileaccess, BitField<ImageFormatLoader::LoaderFlags> p_flags, float p_scale) override;
	virtual void get_recognized_extensions(List<String> *p_extensions) const override;
};

#endif // IMAGE_LOADER_SVG_H
