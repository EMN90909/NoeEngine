// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef LIGHTMAP_RAYCASTER_EMBREE_H
#define LIGHTMAP_RAYCASTER_EMBREE_H

#ifdef TOOLS_ENABLED

#include "core/io/image.h"
#include "core/object/object.h"
#include "scene/3d/lightmapper.h"

#include <embree3/rtcore.h>

class LightmapRaycasterEmbree : public LightmapRaycaster {
	GDCLASS(LightmapRaycasterEmbree, LightmapRaycaster);

private:
	struct AlphaTextureData {
		Vector<uint8_t> data;
		Vector2i size;

		uint8_t sample(float u, float v) const;
	};

	RTCDevice embree_device;
	RTCScene embree_scene;

	static void filter_function(const struct RTCFilterFunctionNArguments *p_args);

	HashMap<unsigned int, AlphaTextureData> alpha_textures;
	HashSet<int> filter_meshes;

public:
	virtual bool intersect(Ray &p_ray) override;

	virtual void intersect(Vector<Ray> &r_rays) override;

	virtual void add_mesh(const Vector<Vector3> &p_vertices, const Vector<Vector3> &p_normals, const Vector<Vector2> &p_uv2s, unsigned int p_id) override;
	virtual void set_mesh_alpha_texture(Ref<Image> p_alpha_texture, unsigned int p_id) override;
	virtual void commit() override;

	virtual void set_mesh_filter(const HashSet<int> &p_mesh_ids) override;
	virtual void clear_mesh_filter() override;

	static LightmapRaycaster *create_embree_raycaster();
	static void make_default_raycaster();

	LightmapRaycasterEmbree();
	~LightmapRaycasterEmbree();
};

#endif // TOOLS_ENABLED

#endif // LIGHTMAP_RAYCASTER_EMBREE_H
