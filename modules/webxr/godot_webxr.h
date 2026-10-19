// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GODOT_WEBXR_H
#define GODOT_WEBXR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

enum WebXRInputEvent {
	WEBXR_INPUT_EVENT_SELECTSTART,
	WEBXR_INPUT_EVENT_SELECTEND,
	WEBXR_INPUT_EVENT_SQUEEZESTART,
	WEBXR_INPUT_EVENT_SQUEEZEEND,
};

typedef void (*GodotWebXRSupportedCallback)(char *p_session_mode, int p_supported);
typedef void (*GodotWebXRStartedCallback)(char *p_reference_space_type);
typedef void (*GodotWebXREndedCallback)();
typedef void (*GodotWebXRFailedCallback)(char *p_message);
typedef void (*GodotWebXRInputEventCallback)(int p_event_type, int p_input_source_id);
typedef void (*GodotWebXRSimpleEventCallback)(char *p_signal_name);

extern int godot_webxr_is_supported();
extern void godot_webxr_is_session_supported(const char *p_session_mode, GodotWebXRSupportedCallback p_callback);

extern void godot_webxr_initialize(
		const char *p_session_mode,
		const char *p_required_features,
		const char *p_optional_features,
		const char *p_requested_reference_space_types,
		GodotWebXRStartedCallback p_on_session_started,
		GodotWebXREndedCallback p_on_session_ended,
		GodotWebXRFailedCallback p_on_session_failed,
		GodotWebXRInputEventCallback p_on_input_event,
		GodotWebXRSimpleEventCallback p_on_simple_event);
extern void godot_webxr_uninitialize();

extern int godot_webxr_get_view_count();
extern bool godot_webxr_get_render_target_size(int *r_size);
extern bool godot_webxr_get_transform_for_view(int p_view, float *r_transform);
extern bool godot_webxr_get_projection_for_view(int p_view, float *r_transform);
extern unsigned int godot_webxr_get_color_texture();
extern unsigned int godot_webxr_get_depth_texture();
extern unsigned int godot_webxr_get_velocity_texture();

extern bool godot_webxr_update_input_source(
		int p_input_source_id,
		float *r_target_pose,
		int *r_target_ray_mode,
		int *r_touch_index,
		int *r_has_grip_pose,
		float *r_grip_pose,
		int *r_has_standard_mapping,
		int *r_button_count,
		float *r_buttons,
		int *r_axes_count,
		float *r_axes);

extern char *godot_webxr_get_visibility_state();
extern int godot_webxr_get_bounds_geometry(float **r_points);

#ifdef __cplusplus
}
#endif

#endif // GODOT_WEBXR_H
