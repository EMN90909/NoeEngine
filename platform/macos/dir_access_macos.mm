// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "dir_access_macos.h"

#if defined(UNIX_ENABLED)

#include <errno.h>

#import <AppKit/NSWorkspace.h>
#import <Foundation/Foundation.h>

String DirAccessMacOS::fix_unicode_name(const char *p_name) const {
	String fname;
	NSString *nsstr = [[NSString stringWithUTF8String:p_name] precomposedStringWithCanonicalMapping];

	fname.parse_utf8([nsstr UTF8String]);

	return fname;
}

int DirAccessMacOS::get_drive_count() {
	NSArray *res_keys = [NSArray arrayWithObjects:NSURLVolumeURLKey, NSURLIsSystemImmutableKey, nil];
	NSArray *vols = [[NSFileManager defaultManager] mountedVolumeURLsIncludingResourceValuesForKeys:res_keys options:NSVolumeEnumerationSkipHiddenVolumes];

	return [vols count];
}

String DirAccessMacOS::get_drive(int p_drive) {
	NSArray *res_keys = [NSArray arrayWithObjects:NSURLVolumeURLKey, NSURLIsSystemImmutableKey, nil];
	NSArray *vols = [[NSFileManager defaultManager] mountedVolumeURLsIncludingResourceValuesForKeys:res_keys options:NSVolumeEnumerationSkipHiddenVolumes];
	int count = [vols count];

	ERR_FAIL_INDEX_V(p_drive, count, "");

	String volname;
	NSString *path = [vols[p_drive] path];

	volname.parse_utf8([path UTF8String]);

	return volname;
}

bool DirAccessMacOS::is_hidden(const String &p_name) {
	String f = get_current_dir().path_join(p_name);
	NSURL *url = [NSURL fileURLWithPath:@(f.utf8().get_data())];
	NSNumber *hidden = nil;
	if (![url getResourceValue:&hidden forKey:NSURLIsHiddenKey error:nil]) {
		return DirAccessUnix::is_hidden(p_name);
	}
	return [hidden boolValue];
}

#endif // UNIX_ENABLED
