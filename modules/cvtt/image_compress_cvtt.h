// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IMAGE_COMPRESS_CVTT_H
#define IMAGE_COMPRESS_CVTT_H

#include "core/io/image.h"

void image_compress_cvtt(Image *p_image, Image::UsedChannels p_channels);
void image_decompress_cvtt(Image *p_image);

#endif // IMAGE_COMPRESS_CVTT_H
