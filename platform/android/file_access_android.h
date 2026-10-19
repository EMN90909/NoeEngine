// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FILE_ACCESS_ANDROID_H
#define FILE_ACCESS_ANDROID_H

#include "core/io/file_access.h"
#include <android/asset_manager.h>
#include <android/log.h>
#include <stdio.h>

class FileAccessAndroid : public FileAccess {
	mutable AAsset *asset = nullptr;
	mutable uint64_t len = 0;
	mutable uint64_t pos = 0;
	mutable bool eof = false;
	String absolute_path;
	String path_src;

	void _close();

public:
	static AAssetManager *asset_manager;

	virtual Error open_internal(const String &p_path, int p_mode_flags) override; // open a file
	virtual bool is_open() const override; // true when file is open

	/// returns the path for the current open file
	virtual String get_path() const override;
	/// returns the absolute path for the current open file
	virtual String get_path_absolute() const override;

	virtual void seek(uint64_t p_position) override; // seek to a given position
	virtual void seek_end(int64_t p_position = 0) override; // seek from the end of file
	virtual uint64_t get_position() const override; // get position in the file
	virtual uint64_t get_length() const override; // get size of the file

	virtual bool eof_reached() const override; // reading passed EOF

	virtual uint8_t get_8() const override; // get a byte
	virtual uint64_t get_buffer(uint8_t *p_dst, uint64_t p_length) const override;

	virtual Error get_error() const override; // get last error

	virtual void flush() override;
	virtual void store_8(uint8_t p_dest) override; // store a byte

	virtual bool file_exists(const String &p_path) override; // return true if a file exists

	virtual uint64_t _get_modified_time(const String &p_file) override { return 0; }
	virtual uint32_t _get_unix_permissions(const String &p_file) override { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions) override { return FAILED; }

	virtual void close() override;

	~FileAccessAndroid();
};

#endif // FILE_ACCESS_ANDROID_H
