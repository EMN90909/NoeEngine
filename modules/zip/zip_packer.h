// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ZIP_PACKER_H
#define ZIP_PACKER_H

#include "core/io/file_access.h"
#include "core/object/ref_counted.h"

#include "thirdparty/minizip/zip.h"

class ZIPPacker : public RefCounted {
	GDCLASS(ZIPPacker, RefCounted);

	Ref<FileAccess> fa;
	zipFile zf;

protected:
	static void _bind_methods();

public:
	enum ZipAppend {
		APPEND_CREATE = 0,
		APPEND_CREATEAFTER = 1,
		APPEND_ADDINZIP = 2,
	};

	Error open(String p_path, ZipAppend p_append);
	Error close();

	Error start_file(String p_path);
	Error write_file(Vector<uint8_t> p_data);
	Error close_file();

	ZIPPacker();
	~ZIPPacker();
};

VARIANT_ENUM_CAST(ZIPPacker::ZipAppend)

#endif // ZIP_PACKER_H
