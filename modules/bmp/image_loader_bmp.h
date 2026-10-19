// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IMAGE_LOADER_BMP_H
#define IMAGE_LOADER_BMP_H

#include "core/io/image_loader.h"

class ImageLoaderBMP : public ImageFormatLoader {
protected:
	static const unsigned BITMAP_SIGNATURE = 0x4d42;

	static const unsigned BITMAP_FILE_HEADER_SIZE = 14; // bmp_file_header_s
	static const unsigned BITMAP_INFO_HEADER_MIN_SIZE = 40; // bmp_info_header_s

	enum bmp_compression_s {
		BI_RGB = 0x00,
		BI_RLE8 = 0x01, // compressed
		BI_RLE4 = 0x02, // compressed
		BI_BITFIELDS = 0x03,
		BI_JPEG = 0x04,
		BI_PNG = 0x05,
		BI_ALPHABITFIELDS = 0x06,
		BI_CMYK = 0x0b,
		BI_CMYKRLE8 = 0x0c, // compressed
		BI_CMYKRLE4 = 0x0d // compressed
	};

	struct bmp_header_s {
		struct bmp_file_header_s {
			uint16_t bmp_signature = 0;
			uint32_t bmp_file_size = 0;
			uint32_t bmp_file_padding = 0;
			uint32_t bmp_file_offset = 0;
		} bmp_file_header;

		struct bmp_info_header_s {
			uint32_t bmp_header_size = 0;
			uint32_t bmp_width = 0;
			uint32_t bmp_height = 0;
			uint16_t bmp_planes = 0;
			uint16_t bmp_bit_count = 0;
			uint32_t bmp_compression = 0;
			uint32_t bmp_size_image = 0;
			uint32_t bmp_pixels_per_meter_x = 0;
			uint32_t bmp_pixels_per_meter_y = 0;
			uint32_t bmp_colors_used = 0;
			uint32_t bmp_important_colors = 0;
		} bmp_info_header;
	};

	static Error convert_to_image(Ref<Image> p_image,
			const uint8_t *p_buffer,
			const uint8_t *p_color_buffer,
			const uint32_t color_table_size,
			const bmp_header_s &p_header);

public:
	virtual Error load_image(Ref<Image> p_image, Ref<FileAccess> f, BitField<ImageFormatLoader::LoaderFlags> p_flags, float p_scale);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	ImageLoaderBMP();
};

#endif // IMAGE_LOADER_BMP_H
