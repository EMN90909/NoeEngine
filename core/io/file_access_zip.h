// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FILE_ACCESS_ZIP_H
#define FILE_ACCESS_ZIP_H

#ifdef MINIZIP_ENABLED

#include "core/io/file_access_pack.h"
#include "core/templates/rb_map.h"

#include "thirdparty/minizip/unzip.h"

#include <stdlib.h>

class ZipArchive : public PackSource {
public:
	struct File {
		int package = -1;
		unz_file_pos file_pos;
		File() {}
	};

private:
	struct Package {
		String filename;
		unzFile zfile = nullptr;
	};
	Vector<Package> packages;

	HashMap<String, File> files;

	static ZipArchive *instance;

public:
	void close_handle(unzFile p_file) const;
	unzFile get_file_handle(String p_file) const;

	Error add_package(String p_name);

	bool file_exists(String p_name) const;

	virtual bool try_open_pack(const String &p_path, bool p_replace_files, uint64_t p_offset) override;
	Ref<FileAccess> get_file(const String &p_path, PackedData::PackedFile *p_file) override;

	static ZipArchive *get_singleton();

	ZipArchive();
	~ZipArchive();
};

class FileAccessZip : public FileAccess {
	unzFile zfile = nullptr;
	unz_file_info64 file_info;

	mutable bool at_eof = false;

	void _close();

public:
	virtual Error open_internal(const String &p_path, int p_mode_flags) override; ///< open a file
	virtual bool is_open() const override; ///< true when file is open

	virtual void seek(uint64_t p_position) override; ///< seek to a given position
	virtual void seek_end(int64_t p_position = 0) override; ///< seek from the end of file
	virtual uint64_t get_position() const override; ///< get position in the file
	virtual uint64_t get_length() const override; ///< get size of the file

	virtual bool eof_reached() const override; ///< reading passed EOF

	virtual uint8_t get_8() const override; ///< get a byte
	virtual uint64_t get_buffer(uint8_t *p_dst, uint64_t p_length) const override;

	virtual Error get_error() const override; ///< get last error

	virtual void flush() override;
	virtual void store_8(uint8_t p_dest) override; ///< store a byte

	virtual bool file_exists(const String &p_name) override; ///< return true if a file exists

	virtual uint64_t _get_modified_time(const String &p_file) override { return 0; } // todo
	virtual uint32_t _get_unix_permissions(const String &p_file) override { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions) override { return FAILED; }

	virtual void close() override;

	FileAccessZip(const String &p_path, const PackedData::PackedFile &p_file);
	~FileAccessZip();
};

#endif // MINIZIP_ENABLED

#endif // FILE_ACCESS_ZIP_H
