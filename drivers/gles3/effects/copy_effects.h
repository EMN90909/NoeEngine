// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef COPY_EFFECTS_GLES3_H
#define COPY_EFFECTS_GLES3_H

#ifdef GLES3_ENABLED

#include "../shaders/copy.glsl.gen.h"

namespace GLES3 {

class CopyEffects {
private:
	struct Copy {
		CopyShaderGLES3 shader;
		RID shader_version;
	} copy;

	static CopyEffects *singleton;

	// Use for full-screen effects. Slightly more efficient than screen_quad as this eliminates pixel overdraw along the diagonal.
	GLuint screen_triangle = 0;
	GLuint screen_triangle_array = 0;

	// Use for rect-based effects.
	GLuint quad = 0;
	GLuint quad_array = 0;

public:
	static CopyEffects *get_singleton();

	CopyEffects();
	~CopyEffects();

	// These functions assume that a framebuffer and texture are bound already. They only manage the shader, uniforms, and vertex array.
	void copy_to_rect(const Rect2 &p_rect);
	void copy_screen();
	void bilinear_blur(GLuint p_source_texture, int p_mipmap_count, const Rect2i &p_region);
	void set_color(const Color &p_color, const Rect2i &p_region);
	void draw_screen_triangle();
	void draw_screen_quad();
};

} //namespace GLES3

#endif // GLES3_ENABLED

#endif // COPY_EFFECTS_GLES3_H
