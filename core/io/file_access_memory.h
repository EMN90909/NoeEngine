// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FILE_ACCESS_MEMORY_H
#define FILE_ACCESS_MEMORY_H

#include "core/io/file_access.h"

class FileAccessMemory : public FileAccess {
	uint8_t *data = nullptr;
	uint64_t length = 0;
	mutable uint64_t pos = 0;

	static Ref<FileAccess> create();

public:
	static void register_file(String p_name, Vector<uint8_t> p_data);
	static void cleanup();

	virtual Error open_custom(const uint8_t *p_data, uint64_t p_len); ///< open a file
	virtual Error open_internal(const String &p_path, int p_mode_flags) override; ///< open a file
	virtual bool is_open() const override; ///< true when file is open

	virtual void seek(uint64_t p_position) override; ///< seek to a given position
	virtual void seek_end(int64_t p_position) override; ///< seek from the end of file
	virtual uint64_t get_position() const override; ///< get position in the file
	virtual uint64_t get_length() const override; ///< get size of the file

	virtual bool eof_reached() const override; ///< reading passed EOF

	virtual uint8_t get_8() const override; ///< get a byte

	virtual uint64_t get_buffer(uint8_t *p_dst, uint64_t p_length) const override; ///< get an array of bytes

	virtual Error get_error() const override; ///< get last error

	virtual void flush() override;
	virtual void store_8(uint8_t p_byte) override; ///< store a byte
	virtual void store_buffer(const uint8_t *p_src, uint64_t p_length) override; ///< store an array of bytes

	virtual bool file_exists(const String &p_name) override; ///< return true if a file exists

	virtual uint64_t _get_modified_time(const String &p_file) override { return 0; }
	virtual uint32_t _get_unix_permissions(const String &p_file) override { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions) override { return FAILED; }

	virtual void close() override {}

	FileAccessMemory() {}
};

#endif // FILE_ACCESS_MEMORY_H
