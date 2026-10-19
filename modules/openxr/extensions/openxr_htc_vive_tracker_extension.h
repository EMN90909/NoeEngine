// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef OPENXR_HTC_VIVE_TRACKER_EXTENSION_H
#define OPENXR_HTC_VIVE_TRACKER_EXTENSION_H

#include "openxr_extension_wrapper.h"

class OpenXRHTCViveTrackerExtension : public OpenXRExtensionWrapper {
public:
	virtual HashMap<String, bool *> get_requested_extensions() override;

	bool is_available();

	virtual void on_register_metadata() override;
	virtual bool on_event_polled(const XrEventDataBuffer &event) override;

private:
	bool available = false;
};

#endif // OPENXR_HTC_VIVE_TRACKER_EXTENSION_H
