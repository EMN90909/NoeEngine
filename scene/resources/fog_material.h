// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef FOG_MATERIAL_H
#define FOG_MATERIAL_H

#include "scene/resources/material.h"

class FogMaterial : public Material {
	GDCLASS(FogMaterial, Material);

private:
	float density = 1.0;
	Color albedo = Color(1, 1, 1, 1);
	Color emission = Color(0, 0, 0, 0);

	float height_falloff = 0.0;

	float edge_fade = 0.1;

	Ref<Texture3D> density_texture;

	static Mutex shader_mutex;
	static RID shader;
	static void _update_shader();
	mutable bool shader_set = false;

protected:
	static void _bind_methods();

public:
	void set_density(float p_density);
	float get_density() const;

	void set_albedo(Color p_color);
	Color get_albedo() const;

	void set_emission(Color p_color);
	Color get_emission() const;

	void set_height_falloff(float p_falloff);
	float get_height_falloff() const;

	void set_edge_fade(float p_edge_fade);
	float get_edge_fade() const;

	void set_density_texture(const Ref<Texture3D> &p_texture);
	Ref<Texture3D> get_density_texture() const;

	virtual Shader::Mode get_shader_mode() const override;
	virtual RID get_shader_rid() const override;
	virtual RID get_rid() const override;

	static void cleanup_shader();

	FogMaterial();
	virtual ~FogMaterial();
};

#endif // FOG_MATERIAL_H
