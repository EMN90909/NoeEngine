// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ZIP_READER_H
#define ZIP_READER_H

#include "core/io/file_access.h"
#include "core/object/ref_counted.h"

#include "thirdparty/minizip/unzip.h"

class ZIPReader : public RefCounted {
	GDCLASS(ZIPReader, RefCounted)

	Ref<FileAccess> fa;
	unzFile uzf;

protected:
	static void _bind_methods();

public:
	Error open(String p_path);
	Error close();

	PackedStringArray get_files();
	PackedByteArray read_file(String p_path, bool p_case_sensitive);

	ZIPReader();
	~ZIPReader();
};

#endif // ZIP_READER_H
