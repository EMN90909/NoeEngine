// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "resource_importer_bitmask.h"

#include "core/io/image.h"
#include "core/io/image_loader.h"
#include "core/io/resource_saver.h"
#include "scene/resources/bit_map.h"

String ResourceImporterBitMap::get_importer_name() const {
	return "bitmap";
}

String ResourceImporterBitMap::get_visible_name() const {
	return "BitMap";
}

void ResourceImporterBitMap::get_recognized_extensions(List<String> *p_extensions) const {
	ImageLoader::get_recognized_extensions(p_extensions);
}

String ResourceImporterBitMap::get_save_extension() const {
	return "res";
}

String ResourceImporterBitMap::get_resource_type() const {
	return "BitMap";
}

bool ResourceImporterBitMap::get_option_visibility(const String &p_path, const String &p_option, const HashMap<StringName, Variant> &p_options) const {
	return true;
}

int ResourceImporterBitMap::get_preset_count() const {
	return 0;
}

String ResourceImporterBitMap::get_preset_name(int p_idx) const {
	return String();
}

void ResourceImporterBitMap::get_import_options(const String &p_path, List<ImportOption> *r_options, int p_preset) const {
	r_options->push_back(ImportOption(PropertyInfo(Variant::INT, "create_from", PROPERTY_HINT_ENUM, "Black & White,Alpha"), 0));
	r_options->push_back(ImportOption(PropertyInfo(Variant::FLOAT, "threshold", PROPERTY_HINT_RANGE, "0,1,0.01"), 0.5));
}

Error ResourceImporterBitMap::import(const String &p_source_file, const String &p_save_path, const HashMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata) {
	int create_from = p_options["create_from"];
	float threshold = p_options["threshold"];
	Ref<Image> image;
	image.instantiate();
	Error err = ImageLoader::load_image(p_source_file, image);
	if (err != OK) {
		return err;
	}

	int w = image->get_width();
	int h = image->get_height();

	Ref<BitMap> bitmap;
	bitmap.instantiate();
	bitmap->create(Size2(w, h));

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			bool bit;
			Color c = image->get_pixel(j, i);
			if (create_from == 0) { //b&W
				bit = c.get_v() > threshold;
			} else {
				bit = c.a > threshold;
			}

			bitmap->set_bit(j, i, bit);
		}
	}

	return ResourceSaver::save(bitmap, p_save_path + ".res");
}

ResourceImporterBitMap::ResourceImporterBitMap() {
}

ResourceImporterBitMap::~ResourceImporterBitMap() {
}
