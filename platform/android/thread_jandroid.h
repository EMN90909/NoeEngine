// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef THREAD_JANDROID_H
#define THREAD_JANDROID_H

#include <jni.h>

void init_thread_jandroid(JavaVM *p_jvm, JNIEnv *p_env);

void setup_android_thread();
JNIEnv *get_jni_env();

#endif // THREAD_JANDROID_H
