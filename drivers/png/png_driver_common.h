// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef PNG_DRIVER_COMMON_H
#define PNG_DRIVER_COMMON_H

#include "core/io/image.h"

namespace PNGDriverCommon {

// Attempt to load png from buffer (p_source, p_size) into p_image
Error png_to_image(const uint8_t *p_source, size_t p_size, bool p_force_linear, Ref<Image> p_image);

// Append p_image, as a png, to p_buffer.
// Contents of p_buffer is unspecified if error returned.
Error image_to_png(const Ref<Image> &p_image, Vector<uint8_t> &p_buffer);
} // namespace PNGDriverCommon

#endif // PNG_DRIVER_COMMON_H
