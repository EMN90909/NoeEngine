// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef WEBXR_INTERFACE_H
#define WEBXR_INTERFACE_H

#include "servers/xr/xr_interface.h"
#include "servers/xr/xr_positional_tracker.h"

/**
	The WebXR interface is a VR/AR interface that can be used on the web.
*/

class WebXRInterface : public XRInterface {
	GDCLASS(WebXRInterface, XRInterface);

protected:
	static void _bind_methods();

public:
	enum TargetRayMode {
		TARGET_RAY_MODE_UNKNOWN,
		TARGET_RAY_MODE_GAZE,
		TARGET_RAY_MODE_TRACKED_POINTER,
		TARGET_RAY_MODE_SCREEN,
	};

	virtual void is_session_supported(const String &p_session_mode) = 0;
	virtual void set_session_mode(String p_session_mode) = 0;
	virtual String get_session_mode() const = 0;
	virtual void set_required_features(String p_required_features) = 0;
	virtual String get_required_features() const = 0;
	virtual void set_optional_features(String p_optional_features) = 0;
	virtual String get_optional_features() const = 0;
	virtual void set_requested_reference_space_types(String p_requested_reference_space_types) = 0;
	virtual String get_requested_reference_space_types() const = 0;
	virtual String get_reference_space_type() const = 0;
	virtual bool is_input_source_active(int p_input_source_id) const = 0;
	virtual Ref<XRPositionalTracker> get_input_source_tracker(int p_input_source_id) const = 0;
	virtual TargetRayMode get_input_source_target_ray_mode(int p_input_source_id) const = 0;
	virtual String get_visibility_state() const = 0;
};

VARIANT_ENUM_CAST(WebXRInterface::TargetRayMode);

#endif // WEBXR_INTERFACE_H
