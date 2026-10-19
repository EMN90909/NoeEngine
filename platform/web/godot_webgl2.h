// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_WEBGL2_H
#define GODOT_WEBGL2_H

#include "GLES3/gl3.h"
#include "webgl/webgl2.h"

#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_NUM_VIEWS_OVR 0x9630
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_BASE_VIEW_INDEX_OVR 0x9632
#define GL_MAX_VIEWS_OVR 0x9631
#define GL_FRAMEBUFFER_INCOMPLETE_VIEW_TARGETS_OVR 0x9633

#ifdef __cplusplus
extern "C" {
#endif

void godot_webgl2_glFramebufferTextureMultiviewOVR(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint baseViewIndex, GLsizei numViews);

#define glFramebufferTextureMultiviewOVR godot_webgl2_glFramebufferTextureMultiviewOVR

#ifdef __cplusplus
}
#endif

#endif // GODOT_WEBGL2_H
