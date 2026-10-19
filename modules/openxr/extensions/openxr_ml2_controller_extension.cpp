// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "openxr_ml2_controller_extension.h"
#include "../action_map/openxr_interaction_profile_meta_data.h"

HashMap<String, bool *> OpenXRML2ControllerExtension::get_requested_extensions() {
	HashMap<String, bool *> request_extensions;

	request_extensions[XR_ML_ML2_CONTROLLER_INTERACTION_EXTENSION_NAME] = &available;

	return request_extensions;
}

bool OpenXRML2ControllerExtension::is_available() {
	return available;
}

void OpenXRML2ControllerExtension::on_register_metadata() {
	OpenXRInteractionProfileMetaData *metadata = OpenXRInteractionProfileMetaData::get_singleton();
	ERR_FAIL_NULL(metadata);

	// Magic Leap 2 Controller
	const String profile_path = "/interaction_profiles/ml/ml2_controller";
	metadata->register_interaction_profile("Magic Leap 2 controller", "/interaction_profiles/ml/ml2_controller", XR_ML_ML2_CONTROLLER_INTERACTION_EXTENSION_NAME);
	for (const String user_path : { "/user/hand/left", "/user/hand/right" }) {
		metadata->register_io_path(profile_path, "Grip pose", user_path, user_path + "/input/grip/pose", "", OpenXRAction::OPENXR_ACTION_POSE);
		metadata->register_io_path(profile_path, "Aim pose", user_path, user_path + "/input/aim/pose", "", OpenXRAction::OPENXR_ACTION_POSE);

		metadata->register_io_path(profile_path, "Menu click", user_path, user_path + "/input/menu/click", "", OpenXRAction::OPENXR_ACTION_BOOL);
		metadata->register_io_path(profile_path, "Trigger", user_path, user_path + "/input/trigger/value", "", OpenXRAction::OPENXR_ACTION_FLOAT);
		metadata->register_io_path(profile_path, "Trigger click", user_path, user_path + "/input/trigger/click", "", OpenXRAction::OPENXR_ACTION_BOOL);

		metadata->register_io_path(profile_path, "Shoulder click", user_path, user_path + "/input/shoulder/click", "", OpenXRAction::OPENXR_ACTION_BOOL);

		metadata->register_io_path(profile_path, "Trackpad click", user_path, user_path + "/input/trackpad/click", "", OpenXRAction::OPENXR_ACTION_BOOL);
		metadata->register_io_path(profile_path, "Trackpad force", user_path, user_path + "/input/trackpad/force", "", OpenXRAction::OPENXR_ACTION_FLOAT);
		metadata->register_io_path(profile_path, "Trackpad X", user_path, user_path + "/input/trackpad/x", "", OpenXRAction::OPENXR_ACTION_FLOAT);
		metadata->register_io_path(profile_path, "Trackpad Y", user_path, user_path + "/input/trackpad/y", "", OpenXRAction::OPENXR_ACTION_FLOAT);
		metadata->register_io_path(profile_path, "Trackpad touch", user_path, user_path + "/input/trackpad/touch", "", OpenXRAction::OPENXR_ACTION_VECTOR2);

		metadata->register_io_path(profile_path, "Haptic output", user_path, user_path + "/output/haptic", "", OpenXRAction::OPENXR_ACTION_HAPTIC);
	}
}
