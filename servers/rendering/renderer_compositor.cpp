// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "renderer_compositor.h"

#include "core/config/project_settings.h"
#include "core/os/os.h"
#include "core/string/print_string.h"
#include "servers/xr_server.h"

RendererCompositor *RendererCompositor::singleton = nullptr;

RendererCompositor *(*RendererCompositor::_create_func)() = nullptr;
bool RendererCompositor::low_end = false;

RendererCompositor *RendererCompositor::create() {
	return _create_func();
}

bool RendererCompositor::is_xr_enabled() const {
	return xr_enabled;
}

RendererCompositor::RendererCompositor() {
	singleton = this;

	if (XRServer::get_xr_mode() == XRServer::XRMODE_DEFAULT) {
		xr_enabled = GLOBAL_GET("xr/shaders/enabled");
	} else {
		xr_enabled = XRServer::get_xr_mode() == XRServer::XRMODE_ON;
	}
}

RendererCanvasRender *RendererCanvasRender::singleton = nullptr;
