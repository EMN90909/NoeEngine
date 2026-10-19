// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "core/templates/vector.h"
#include "core/typedefs.h"

class Compression {
public:
	static int zlib_level;
	static int gzip_level;
	static int zstd_level;
	static bool zstd_long_distance_matching;
	static int zstd_window_log_size;
	static int gzip_chunk;

	enum Mode {
		MODE_FASTLZ,
		MODE_DEFLATE,
		MODE_ZSTD,
		MODE_GZIP
	};

	static int compress(uint8_t *p_dst, const uint8_t *p_src, int p_src_size, Mode p_mode = MODE_ZSTD);
	static int get_max_compressed_buffer_size(int p_src_size, Mode p_mode = MODE_ZSTD);
	static int decompress(uint8_t *p_dst, int p_dst_max_size, const uint8_t *p_src, int p_src_size, Mode p_mode = MODE_ZSTD);
	static int decompress_dynamic(Vector<uint8_t> *p_dst_vect, int p_max_dst_size, const uint8_t *p_src, int p_src_size, Mode p_mode);
};

#endif // COMPRESSION_H
