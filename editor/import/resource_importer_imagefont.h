// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RESOURCE_IMPORTER_IMAGEFONT_H
#define RESOURCE_IMPORTER_IMAGEFONT_H

#include "core/io/resource_importer.h"
#include "scene/resources/font.h"
#include "servers/text_server.h"

class ResourceImporterImageFont : public ResourceImporter {
	GDCLASS(ResourceImporterImageFont, ResourceImporter);

public:
	static bool _decode_range(const String &p_token, int32_t &r_pos);

	virtual String get_importer_name() const override;
	virtual String get_visible_name() const override;
	virtual void get_recognized_extensions(List<String> *p_extensions) const override;
	virtual String get_save_extension() const override;
	virtual String get_resource_type() const override;

	virtual void get_import_options(const String &p_path, List<ImportOption> *r_options, int p_preset = 0) const override;
	virtual bool get_option_visibility(const String &p_path, const String &p_option, const HashMap<StringName, Variant> &p_options) const override;

	virtual Error import(const String &p_source_file, const String &p_save_path, const HashMap<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files = nullptr, Variant *r_metadata = nullptr) override;

	ResourceImporterImageFont();
};

#endif // RESOURCE_IMPORTER_IMAGEFONT_H
