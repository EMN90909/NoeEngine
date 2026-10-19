// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IMAGE_SAVER_TINYEXR_H
#define IMAGE_SAVER_TINYEXR_H

#include "core/os/os.h"

Error save_exr(const String &p_path, const Ref<Image> &p_img, bool p_grayscale);
Vector<uint8_t> save_exr_buffer(const Ref<Image> &p_img, bool p_grayscale);

#endif // IMAGE_SAVER_TINYEXR_H
