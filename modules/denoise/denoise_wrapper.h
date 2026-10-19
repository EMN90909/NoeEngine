// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef DENOISE_WRAPPER_H
#define DENOISE_WRAPPER_H

void *oidn_denoiser_init();
bool oidn_denoise(void *device, float *p_floats, int p_width, int p_height);
void oidn_denoiser_finish(void *device);

#endif // DENOISE_WRAPPER_H
