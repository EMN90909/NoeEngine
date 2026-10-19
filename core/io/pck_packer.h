// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PCK_PACKER_H
#define PCK_PACKER_H

#include "core/object/ref_counted.h"

class FileAccess;

class PCKPacker : public RefCounted {
	GDCLASS(PCKPacker, RefCounted);

	Ref<FileAccess> file;
	int alignment = 0;
	uint64_t ofs = 0;

	Vector<uint8_t> key;
	bool enc_dir = false;

	static void _bind_methods();

	struct File {
		String path;
		String src_path;
		uint64_t ofs = 0;
		uint64_t size = 0;
		bool encrypted = false;
		Vector<uint8_t> md5;
	};
	Vector<File> files;

public:
	Error pck_start(const String &p_file, int p_alignment = 32, const String &p_key = "0000000000000000000000000000000000000000000000000000000000000000", bool p_encrypt_directory = false);
	Error add_file(const String &p_file, const String &p_src, bool p_encrypt = false);
	Error flush(bool p_verbose = false);

	PCKPacker() {}
};

#endif // PCK_PACKER_H
