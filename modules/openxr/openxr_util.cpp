// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "openxr_util.h"
#include <openxr/openxr_reflection.h>

#define XR_ENUM_CASE_STR(name, val) \
	case name:                      \
		return #name;
#define XR_ENUM_SWITCH(enumType, var)                                                                                           \
	switch (var) {                                                                                                              \
		XR_LIST_ENUM_##enumType(XR_ENUM_CASE_STR) default : return "Unknown " #enumType ": " + String::num_int64(int64_t(var)); \
	}

String OpenXRUtil::get_view_configuration_name(XrViewConfigurationType p_view_configuration){
	XR_ENUM_SWITCH(XrViewConfigurationType, p_view_configuration)
}

String OpenXRUtil::get_reference_space_name(XrReferenceSpaceType p_reference_space){
	XR_ENUM_SWITCH(XrReferenceSpaceType, p_reference_space)
}

String OpenXRUtil::get_structure_type_name(XrStructureType p_structure_type){
	XR_ENUM_SWITCH(XrStructureType, p_structure_type)
}

String OpenXRUtil::get_session_state_name(XrSessionState p_session_state){
	XR_ENUM_SWITCH(XrSessionState, p_session_state)
}

String OpenXRUtil::get_action_type_name(XrActionType p_action_type){
	XR_ENUM_SWITCH(XrActionType, p_action_type)
}

String OpenXRUtil::get_environment_blend_mode_name(XrEnvironmentBlendMode p_blend_mode) {
	XR_ENUM_SWITCH(XrEnvironmentBlendMode, p_blend_mode);
}

String OpenXRUtil::make_xr_version_string(XrVersion p_version) {
	String version;

	version += String::num_int64(XR_VERSION_MAJOR(p_version));
	version += String(".");
	version += String::num_int64(XR_VERSION_MINOR(p_version));
	version += String(".");
	version += String::num_int64(XR_VERSION_PATCH(p_version));

	return version;
}
