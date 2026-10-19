// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#import <GameController/GameController.h>

@interface JoypadIOSObserver : NSObject

- (void)startObserving;
- (void)startProcessing;
- (void)finishObserving;

@end

class JoypadIOS {
private:
	JoypadIOSObserver *observer;

public:
	JoypadIOS();
	~JoypadIOS();

	void start_processing();
};
