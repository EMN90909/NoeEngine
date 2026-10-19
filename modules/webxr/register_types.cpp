// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "register_types.h"

#include "webxr_interface.h"
#include "webxr_interface_js.h"

#ifdef WEB_ENABLED
Ref<WebXRInterfaceJS> webxr;
#endif

void initialize_webxr_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_ABSTRACT_CLASS(WebXRInterface);

#ifdef WEB_ENABLED
	webxr.instantiate();
	XRServer::get_singleton()->add_interface(webxr);
#endif
}

void uninitialize_webxr_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

#ifdef WEB_ENABLED
	if (webxr.is_valid()) {
		// uninitialize our interface if it is initialized
		if (webxr->is_initialized()) {
			webxr->uninitialize();
		}

		// unregister our interface from the XR server
		if (XRServer::get_singleton()) {
			XRServer::get_singleton()->remove_interface(webxr);
		}

		// and release
		webxr.unref();
	}
#endif
}
