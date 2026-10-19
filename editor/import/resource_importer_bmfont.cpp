// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "resource_importer_bmfont.h"

#include "core/io/resource_saver.h"

String ResourceImporterBMFont::get_importer_name() const {
	return "font_data_bmfont";
}

String ResourceImporterBMFont::get_visible_name() const {
	return "Font Data (AngelCode BMFont)";
}

void ResourceImporterBMFont::get_recognized_extensions(List<String> *p_extensions) const {
	if (p_extensions) {
		p_extensions->push_back("font");
		p_extensions->push_back("fnt");
	}
}

String ResourceImporterBMFont::get_save_extension() const {
	return "fontdata";
}

String ResourceImporterBMFont::get_resource_type() const {
	return "FontFile";
}

bool ResourceImporterBMFont::get_option_visibility(const String &p_path, const String &p_option, const HashMap<StringName, Variant> &p_options) const {
	return true;
}

void ResourceImporterBMFont::get_import_options(const String &p_path, List<ImportOption> *r_options, int p_preset) const {
	r_options->push_back(ImportOption(PropertyInfo(Variant::ARRAY, "fallbacks", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT("Font")), Array()));

	r_options->push_back(ImportOption(PropertyInfo(Variant::BOOL, "compress"), true));
}

Error ResourceImporterBMFont::import(const String &p_source_file, const String &p_save_path, const HashMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata) {
	print_verbose("Importing BMFont font from: " + p_source_file);

	Array fallbacks = p_options["fallbacks"];

	Ref<FontFile> font;
	font.instantiate();

	Error err = font->load_bitmap_font(p_source_file);
	ERR_FAIL_COND_V_MSG(err != OK, err, "Cannot load font to file \"" + p_source_file + "\".");

	font->set_allow_system_fallback(false);
	font->set_fallbacks(fallbacks);

	int flg = 0;
	if ((bool)p_options["compress"]) {
		flg |= ResourceSaver::SaverFlags::FLAG_COMPRESS;
	}

	print_verbose("Saving to: " + p_save_path + ".fontdata");
	err = ResourceSaver::save(font, p_save_path + ".fontdata", flg);
	ERR_FAIL_COND_V_MSG(err != OK, err, "Cannot save font to file \"" + p_save_path + ".res\".");
	print_verbose("Done saving to: " + p_save_path + ".fontdata");
	return OK;
}

ResourceImporterBMFont::ResourceImporterBMFont() {
}
