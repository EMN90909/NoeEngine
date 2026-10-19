// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "macos_utils.h"

#ifdef MACOS_ENABLED

#include "core/string/print_string.h"

#import <CoreFoundation/CoreFoundation.h>
#import <CoreServices/CoreServices.h>

bool macos_is_app_bundle_installed(const String &p_bundle_id) {
	CFStringRef bundle_id = CFStringCreateWithCString(nullptr, p_bundle_id.utf8(), kCFStringEncodingUTF8);
	CFArrayRef result = LSCopyApplicationURLsForBundleIdentifier(bundle_id, nullptr);
	CFRelease(bundle_id);

	if (result) {
		if (CFArrayGetCount(result) > 0) {
			CFRelease(result);
			return true;
		} else {
			CFRelease(result);
			return false;
		}
	} else {
		return false;
	}
}

#endif
