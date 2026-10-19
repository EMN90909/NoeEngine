// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef SIMPLE_TYPE_H
#define SIMPLE_TYPE_H

/* Batch of specializations to obtain the actual simple type */

template <class T>
struct GetSimpleTypeT {
	typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T &> {
	typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T const> {
	typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T const &> {
	typedef T type_t;
};

#endif // SIMPLE_TYPE_H
