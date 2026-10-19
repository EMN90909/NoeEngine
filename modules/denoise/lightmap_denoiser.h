// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef LIGHTMAP_DENOISER_H
#define LIGHTMAP_DENOISER_H

#include "core/object/class_db.h"
#include "scene/3d/lightmapper.h"

struct OIDNDeviceImpl;

class LightmapDenoiserOIDN : public LightmapDenoiser {
	GDCLASS(LightmapDenoiserOIDN, LightmapDenoiser);

protected:
	void *device = nullptr;

public:
	static LightmapDenoiser *create_oidn_denoiser();

	Ref<Image> denoise_image(const Ref<Image> &p_image) override;

	static void make_default_denoiser();

	LightmapDenoiserOIDN();
	~LightmapDenoiserOIDN();
};

#endif // LIGHTMAP_DENOISER_H
