// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IMAGE_COMPRESS_ASTCENC_H
#define IMAGE_COMPRESS_ASTCENC_H

#include "core/io/image.h"

void _compress_astc(Image *r_img, Image::ASTCFormat p_format);
void _decompress_astc(Image *r_img);

#endif // IMAGE_COMPRESS_ASTCENC_H
