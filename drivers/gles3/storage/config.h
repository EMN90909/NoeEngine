// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CONFIG_GLES3_H
#define CONFIG_GLES3_H

#ifdef GLES3_ENABLED

#include "core/string/ustring.h"
#include "core/templates/hash_set.h"

// This must come first to avoid windows.h mess
#include "platform_config.h"
#ifndef OPENGL_INCLUDE_H
#include <GLES3/gl3.h>
#else
#include OPENGL_INCLUDE_H
#endif

#ifdef ANDROID_ENABLED
typedef void (*PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC)(GLenum, GLenum, GLuint, GLint, GLint, GLsizei);
#endif

namespace GLES3 {

class Config {
private:
	static Config *singleton;

public:
	bool use_nearest_mip_filter = false;
	bool use_depth_prepass = true;

	int max_vertex_texture_image_units = 0;
	int max_texture_image_units = 0;
	int max_texture_size = 0;
	int max_viewport_size[2] = { 0, 0 };
	int max_uniform_buffer_size = 0;
	int max_renderable_elements = 0;
	int max_renderable_lights = 0;
	int max_lights_per_object = 0;

	// TODO implement wireframe in OpenGL
	// bool generate_wireframes;

	HashSet<String> extensions;

	bool float_texture_supported = false;
	bool s3tc_supported = false;
	bool rgtc_supported = false;
	bool bptc_supported = false;
	bool etc2_supported = false;
	bool astc_supported = false;
	bool astc_hdr_supported = false;
	bool astc_layered_supported = false;

	bool force_vertex_shading = false;

	bool support_anisotropic_filter = false;
	float anisotropic_level = 0.0f;

	bool multiview_supported = false;
#ifdef ANDROID_ENABLED
	PFNGLFRAMEBUFFERTEXTUREMULTIVIEWOVRPROC eglFramebufferTextureMultiviewOVR = nullptr;
#endif

	static Config *get_singleton() { return singleton; };

	Config();
	~Config();
};

} // namespace GLES3

#endif // GLES3_ENABLED

#endif // CONFIG_GLES3_H
