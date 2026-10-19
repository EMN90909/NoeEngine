// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_TEXTURE_H
#define GLTF_TEXTURE_H

#include "../gltf_defines.h"
#include "core/io/resource.h"

class GLTFTexture : public Resource {
	GDCLASS(GLTFTexture, Resource);

private:
	GLTFImageIndex src_image = 0;
	GLTFTextureSamplerIndex sampler = -1;

protected:
	static void _bind_methods();

public:
	GLTFImageIndex get_src_image() const;
	void set_src_image(GLTFImageIndex val);
	GLTFTextureSamplerIndex get_sampler() const;
	void set_sampler(GLTFTextureSamplerIndex val);
};

#endif // GLTF_TEXTURE_H
