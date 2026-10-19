// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#include "ios.h"

#import "app_delegate.h"
#import "view_controller.h"

#import <CoreHaptics/CoreHaptics.h>
#import <UIKit/UIKit.h>
#include <sys/sysctl.h>

void iOS::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_rate_url", "app_id"), &iOS::get_rate_url);
	ClassDB::bind_method(D_METHOD("supports_haptic_engine"), &iOS::supports_haptic_engine);
	ClassDB::bind_method(D_METHOD("start_haptic_engine"), &iOS::start_haptic_engine);
	ClassDB::bind_method(D_METHOD("stop_haptic_engine"), &iOS::stop_haptic_engine);
};

bool iOS::supports_haptic_engine() {
	if (@available(iOS 13, *)) {
		id<CHHapticDeviceCapability> capabilities = [CHHapticEngine capabilitiesForHardware];
		return capabilities.supportsHaptics;
	}

	return false;
}

CHHapticEngine *iOS::get_haptic_engine_instance() API_AVAILABLE(ios(13)) {
	if (haptic_engine == nullptr) {
		NSError *error = nullptr;
		haptic_engine = [[CHHapticEngine alloc] initAndReturnError:&error];

		if (!error) {
			[haptic_engine setAutoShutdownEnabled:true];
		} else {
			haptic_engine = nullptr;
			NSLog(@"Could not initialize haptic engine: %@", error);
		}
	}

	return haptic_engine;
}

void iOS::vibrate_haptic_engine(float p_duration_seconds) API_AVAILABLE(ios(13)) {
	if (@available(iOS 13, *)) { // We need the @available check every time to make the compiler happy...
		if (supports_haptic_engine()) {
			CHHapticEngine *cur_haptic_engine = get_haptic_engine_instance();
			if (cur_haptic_engine) {
				NSDictionary *hapticDict = @{
					CHHapticPatternKeyPattern : @[
						@{CHHapticPatternKeyEvent : @{
							CHHapticPatternKeyEventType : CHHapticEventTypeHapticContinuous,
							CHHapticPatternKeyTime : @(CHHapticTimeImmediate),
							CHHapticPatternKeyEventDuration : @(p_duration_seconds)
						},
						},
					],
				};

				NSError *error;
				CHHapticPattern *pattern = [[CHHapticPattern alloc] initWithDictionary:hapticDict error:&error];

				[[cur_haptic_engine createPlayerWithPattern:pattern error:&error] startAtTime:0 error:&error];

				NSLog(@"Could not vibrate using haptic engine: %@", error);
			}

			return;
		}
	}

	NSLog(@"Haptic engine is not supported in this version of iOS");
}

void iOS::start_haptic_engine() {
	if (@available(iOS 13, *)) {
		if (supports_haptic_engine()) {
			CHHapticEngine *cur_haptic_engine = get_haptic_engine_instance();
			if (cur_haptic_engine) {
				[cur_haptic_engine startWithCompletionHandler:^(NSError *returnedError) {
					if (returnedError) {
						NSLog(@"Could not start haptic engine: %@", returnedError);
					}
				}];
			}

			return;
		}
	}

	NSLog(@"Haptic engine is not supported in this version of iOS");
}

void iOS::stop_haptic_engine() {
	if (@available(iOS 13, *)) {
		if (supports_haptic_engine()) {
			CHHapticEngine *cur_haptic_engine = get_haptic_engine_instance();
			if (cur_haptic_engine) {
				[cur_haptic_engine stopWithCompletionHandler:^(NSError *returnedError) {
					if (returnedError) {
						NSLog(@"Could not stop haptic engine: %@", returnedError);
					}
				}];
			}

			return;
		}
	}

	NSLog(@"Haptic engine is not supported in this version of iOS");
}

void iOS::alert(const char *p_alert, const char *p_title) {
	NSString *title = [NSString stringWithUTF8String:p_title];
	NSString *message = [NSString stringWithUTF8String:p_alert];

	UIAlertController *alert = [UIAlertController alertControllerWithTitle:title message:message preferredStyle:UIAlertControllerStyleAlert];
	UIAlertAction *button = [UIAlertAction actionWithTitle:@"OK"
													 style:UIAlertActionStyleCancel
												   handler:^(id){
												   }];

	[alert addAction:button];

	[AppDelegate.viewController presentViewController:alert animated:YES completion:nil];
}

String iOS::get_model() const {
	// [[UIDevice currentDevice] model] only returns "iPad" or "iPhone".
	size_t size;
	sysctlbyname("hw.machine", nullptr, &size, nullptr, 0);
	char *model = (char *)malloc(size);
	if (model == nullptr) {
		return "";
	}
	sysctlbyname("hw.machine", model, &size, nullptr, 0);
	NSString *platform = [NSString stringWithCString:model encoding:NSUTF8StringEncoding];
	free(model);
	const char *str = [platform UTF8String];
	return String::utf8(str != nullptr ? str : "");
}

String iOS::get_rate_url(int p_app_id) const {
	String app_url_path = "itms-apps://itunes.apple.com/app/idAPP_ID";

	String ret = app_url_path.replace("APP_ID", String::num(p_app_id));

	printf("returning rate url %s\n", ret.utf8().get_data());
	return ret;
}

iOS::iOS() {}
