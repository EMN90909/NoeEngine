// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "resource_importer_csv_translation.h"

#include "core/io/file_access.h"
#include "core/io/resource_saver.h"
#include "core/string/optimized_translation.h"
#include "core/string/translation.h"

String ResourceImporterCSVTranslation::get_importer_name() const {
	return "csv_translation";
}

String ResourceImporterCSVTranslation::get_visible_name() const {
	return "CSV Translation";
}

void ResourceImporterCSVTranslation::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("csv");
}

String ResourceImporterCSVTranslation::get_save_extension() const {
	return ""; //does not save a single resource
}

String ResourceImporterCSVTranslation::get_resource_type() const {
	return "Translation";
}

bool ResourceImporterCSVTranslation::get_option_visibility(const String &p_path, const String &p_option, const HashMap<StringName, Variant> &p_options) const {
	return true;
}

int ResourceImporterCSVTranslation::get_preset_count() const {
	return 0;
}

String ResourceImporterCSVTranslation::get_preset_name(int p_idx) const {
	return "";
}

void ResourceImporterCSVTranslation::get_import_options(const String &p_path, List<ImportOption> *r_options, int p_preset) const {
	r_options->push_back(ImportOption(PropertyInfo(Variant::BOOL, "compress"), true));
	r_options->push_back(ImportOption(PropertyInfo(Variant::INT, "delimiter", PROPERTY_HINT_ENUM, "Comma,Semicolon,Tab"), 0));
}

Error ResourceImporterCSVTranslation::import(const String &p_source_file, const String &p_save_path, const HashMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files, Variant *r_metadata) {
	bool compress = p_options["compress"];

	String delimiter;
	switch ((int)p_options["delimiter"]) {
		case 0:
			delimiter = ",";
			break;
		case 1:
			delimiter = ";";
			break;
		case 2:
			delimiter = "\t";
			break;
	}

	Ref<FileAccess> f = FileAccess::open(p_source_file, FileAccess::READ);
	ERR_FAIL_COND_V_MSG(f.is_null(), ERR_INVALID_PARAMETER, "Cannot open file from path '" + p_source_file + "'.");

	Vector<String> line = f->get_csv_line(delimiter);
	ERR_FAIL_COND_V(line.size() <= 1, ERR_PARSE_ERROR);

	Vector<String> locales;
	Vector<Ref<Translation>> translations;

	for (int i = 1; i < line.size(); i++) {
		String locale = TranslationServer::get_singleton()->standardize_locale(line[i]);

		locales.push_back(locale);
		Ref<Translation> translation;
		translation.instantiate();
		translation->set_locale(locale);
		translations.push_back(translation);
	}

	line = f->get_csv_line(delimiter);

	while (line.size() == locales.size() + 1) {
		String key = line[0];
		if (!key.is_empty()) {
			for (int i = 1; i < line.size(); i++) {
				translations.write[i - 1]->add_message(key, line[i].c_unescape());
			}
		}

		line = f->get_csv_line(delimiter);
	}

	for (int i = 0; i < translations.size(); i++) {
		Ref<Translation> xlt = translations[i];

		if (compress) {
			Ref<OptimizedTranslation> cxl = memnew(OptimizedTranslation);
			cxl->generate(xlt);
			xlt = cxl;
		}

		String save_path = p_source_file.get_basename() + "." + translations[i]->get_locale() + ".translation";

		ResourceSaver::save(xlt, save_path);
		if (r_gen_files) {
			r_gen_files->push_back(save_path);
		}
	}

	return OK;
}

ResourceImporterCSVTranslation::ResourceImporterCSVTranslation() {
}
