// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef JNI_UTILS_H
#define JNI_UTILS_H

#include "string_android.h"
#include <core/config/engine.h>
#include <core/variant/variant.h>
#include <jni.h>

struct jvalret {
	jobject obj;
	jvalue val;
	jvalret() { obj = nullptr; }
};

jvalret _variant_to_jvalue(JNIEnv *env, Variant::Type p_type, const Variant *p_arg, bool force_jobject = false);

String _get_class_name(JNIEnv *env, jclass cls, bool *array);

Variant _jobject_to_variant(JNIEnv *env, jobject obj);

Variant::Type get_jni_type(const String &p_type);

const char *get_jni_sig(const String &p_type);

#endif // JNI_UTILS_H
