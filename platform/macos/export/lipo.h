// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MACOS_LIPO_H
#define MACOS_LIPO_H

// Universal / Universal 2 fat binary file creator and extractor.

#include "core/io/file_access.h"
#include "core/object/ref_counted.h"

#include "macho.h"

class LipO : public RefCounted {
	struct FatArch {
		uint32_t cputype;
		uint32_t cpusubtype;
		uint64_t offset;
		uint64_t size;
		uint32_t align;
	};

	Ref<FileAccess> fa;
	Vector<FatArch> archs;

	static inline size_t PAD(size_t s, size_t a) {
		return (a - s % a);
	}

public:
	static bool is_lipo(const String &p_path);

	bool create_file(const String &p_output_path, const PackedStringArray &p_files);

	bool open_file(const String &p_path);
	int get_arch_count() const;
	bool extract_arch(int p_index, const String &p_path);

	void close();

	~LipO();
};

#endif // MACOS_LIPO_H
