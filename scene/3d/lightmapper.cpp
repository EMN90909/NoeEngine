// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "lightmapper.h"

LightmapDenoiser *(*LightmapDenoiser::create_function)() = nullptr;

Ref<LightmapDenoiser> LightmapDenoiser::create() {
	if (create_function) {
		return Ref<LightmapDenoiser>(create_function());
	}
	return Ref<LightmapDenoiser>();
}

LightmapRaycaster *(*LightmapRaycaster::create_function)() = nullptr;

Ref<LightmapRaycaster> LightmapRaycaster::create() {
	if (create_function) {
		return Ref<LightmapRaycaster>(create_function());
	}
	return Ref<LightmapRaycaster>();
}

Lightmapper::CreateFunc Lightmapper::create_custom = nullptr;
Lightmapper::CreateFunc Lightmapper::create_gpu = nullptr;
Lightmapper::CreateFunc Lightmapper::create_cpu = nullptr;

Ref<Lightmapper> Lightmapper::create() {
	Lightmapper *lm = nullptr;
	if (create_custom) {
		lm = create_custom();
	}

	if (!lm && create_gpu) {
		lm = create_gpu();
	}

	if (!lm && create_cpu) {
		lm = create_cpu();
	}
	if (!lm) {
		return Ref<Lightmapper>();
	} else {
		return Ref<Lightmapper>(lm);
	}
}

Lightmapper::Lightmapper() {
}
