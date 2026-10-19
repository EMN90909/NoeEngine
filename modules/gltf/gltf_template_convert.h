// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GLTF_TEMPLATE_CONVERT_H
#define GLTF_TEMPLATE_CONVERT_H

#include "core/templates/hash_set.h"
#include "core/variant/array.h"
#include "core/variant/dictionary.h"

namespace GLTFTemplateConvert {
template <class T>
static Array to_array(const Vector<T> &p_inp) {
	Array ret;
	for (int i = 0; i < p_inp.size(); i++) {
		ret.push_back(p_inp[i]);
	}
	return ret;
}

template <class T>
static TypedArray<T> to_array(const HashSet<T> &p_inp) {
	TypedArray<T> ret;
	typename HashSet<T>::Iterator elem = p_inp.begin();
	while (elem) {
		ret.push_back(*elem);
		++elem;
	}
	return ret;
}

template <class T>
static void set_from_array(Vector<T> &r_out, const Array &p_inp) {
	r_out.clear();
	for (int i = 0; i < p_inp.size(); i++) {
		r_out.push_back(p_inp[i]);
	}
}

template <class T>
static void set_from_array(HashSet<T> &r_out, const TypedArray<T> &p_inp) {
	r_out.clear();
	for (int i = 0; i < p_inp.size(); i++) {
		r_out.insert(p_inp[i]);
	}
}

template <class K, class V>
static Dictionary to_dict(const HashMap<K, V> &p_inp) {
	Dictionary ret;
	for (const KeyValue<K, V> &E : p_inp) {
		ret[E.key] = E.value;
	}
	return ret;
}

template <class K, class V>
static void set_from_dict(HashMap<K, V> &r_out, const Dictionary &p_inp) {
	r_out.clear();
	Array keys = p_inp.keys();
	for (int i = 0; i < keys.size(); i++) {
		r_out[keys[i]] = p_inp[keys[i]];
	}
}
} //namespace GLTFTemplateConvert

#endif // GLTF_TEMPLATE_CONVERT_H
