// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_HAND_TRACKING_EXTENSION_H
#define OPENXR_HAND_TRACKING_EXTENSION_H

#include "openxr_extension_wrapper.h"

#include "../util.h"

#define MAX_OPENXR_TRACKED_HANDS 2

class OpenXRHandTrackingExtension : public OpenXRExtensionWrapper {
public:
	struct HandTracker {
		bool is_initialized = false;
		XrHandJointsMotionRangeEXT motion_range = XR_HAND_JOINTS_MOTION_RANGE_UNOBSTRUCTED_EXT;

		XrHandTrackerEXT hand_tracker = XR_NULL_HANDLE;
		XrHandJointLocationEXT joint_locations[XR_HAND_JOINT_COUNT_EXT];
		XrHandJointVelocityEXT joint_velocities[XR_HAND_JOINT_COUNT_EXT];

		XrHandTrackingAimStateFB aimState;
		XrHandJointVelocitiesEXT velocities;
		XrHandJointLocationsEXT locations;
	};

	static OpenXRHandTrackingExtension *get_singleton();

	OpenXRHandTrackingExtension();
	virtual ~OpenXRHandTrackingExtension() override;

	virtual HashMap<String, bool *> get_requested_extensions() override;

	virtual void on_instance_created(const XrInstance p_instance) override;
	virtual void on_instance_destroyed() override;
	virtual void on_session_destroyed() override;

	virtual void *set_system_properties_and_get_next_pointer(void *p_next_pointer) override;
	virtual void on_state_ready() override;
	virtual void on_process() override;
	virtual void on_state_stopping() override;

	bool get_active();
	const HandTracker *get_hand_tracker(uint32_t p_hand) const;

	XrHandJointsMotionRangeEXT get_motion_range(uint32_t p_hand) const;
	void set_motion_range(uint32_t p_hand, XrHandJointsMotionRangeEXT p_motion_range);

private:
	static OpenXRHandTrackingExtension *singleton;

	// state
	XrSystemHandTrackingPropertiesEXT handTrackingSystemProperties;
	HandTracker hand_trackers[MAX_OPENXR_TRACKED_HANDS]; // Fixed for left and right hand

	// related extensions
	bool hand_tracking_ext = false;
	bool hand_motion_range_ext = false;
	bool hand_tracking_aim_state_ext = false;

	// functions
	void cleanup_hand_tracking();

	// OpenXR API call wrappers
	EXT_PROTO_XRRESULT_FUNC3(xrCreateHandTrackerEXT, (XrSession), p_session, (const XrHandTrackerCreateInfoEXT *), p_createInfo, (XrHandTrackerEXT *), p_handTracker)
	EXT_PROTO_XRRESULT_FUNC1(xrDestroyHandTrackerEXT, (XrHandTrackerEXT), p_handTracker)
	EXT_PROTO_XRRESULT_FUNC3(xrLocateHandJointsEXT, (XrHandTrackerEXT), p_handTracker, (const XrHandJointsLocateInfoEXT *), p_locateInfo, (XrHandJointLocationsEXT *), p_locations)
};

#endif // OPENXR_HAND_TRACKING_EXTENSION_H
