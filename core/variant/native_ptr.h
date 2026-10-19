// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef NATIVE_PTR_H
#define NATIVE_PTR_H

#include "core/math/audio_frame.h"
#include "core/variant/method_ptrcall.h"
#include "core/variant/type_info.h"

template <class T>
struct GDExtensionConstPtr {
	const T *data = nullptr;
	GDExtensionConstPtr(const T *p_assign) { data = p_assign; }
	static const char *get_name() { return "const void"; }
	operator const T *() const { return data; }
	operator Variant() const { return uint64_t(data); }
};

template <class T>
struct GDExtensionPtr {
	T *data = nullptr;
	GDExtensionPtr(T *p_assign) { data = p_assign; }
	static const char *get_name() { return "void"; }
	operator T *() const { return data; }
	operator Variant() const { return uint64_t(data); }
};

#define GDVIRTUAL_NATIVE_PTR(m_type)                                                                 \
	template <>                                                                                      \
	struct GDExtensionConstPtr<const m_type> {                                                       \
		const m_type *data = nullptr;                                                                \
		GDExtensionConstPtr() {}                                                                     \
		GDExtensionConstPtr(const m_type *p_assign) { data = p_assign; }                             \
		static const char *get_name() { return "const " #m_type; }                                   \
		operator const m_type *() const { return data; }                                             \
		operator Variant() const { return uint64_t(data); }                                          \
	};                                                                                               \
	template <>                                                                                      \
	struct VariantCaster<GDExtensionConstPtr<const m_type>> {                                        \
		static _FORCE_INLINE_ GDExtensionConstPtr<const m_type> cast(const Variant &p_variant) {     \
			return GDExtensionConstPtr<const m_type>((const m_type *)p_variant.operator uint64_t()); \
		}                                                                                            \
	};                                                                                               \
	template <>                                                                                      \
	struct GDExtensionPtr<m_type> {                                                                  \
		m_type *data = nullptr;                                                                      \
		GDExtensionPtr() {}                                                                          \
		GDExtensionPtr(m_type *p_assign) { data = p_assign; }                                        \
		static const char *get_name() { return #m_type; }                                            \
		operator m_type *() const { return data; }                                                   \
		operator Variant() const { return uint64_t(data); }                                          \
	};                                                                                               \
	template <>                                                                                      \
	struct VariantCaster<GDExtensionPtr<m_type>> {                                                   \
		static _FORCE_INLINE_ GDExtensionPtr<m_type> cast(const Variant &p_variant) {                \
			return GDExtensionPtr<m_type>((m_type *)p_variant.operator uint64_t());                  \
		}                                                                                            \
	};

template <class T>
struct GetTypeInfo<GDExtensionConstPtr<T>> {
	static const Variant::Type VARIANT_TYPE = Variant::NIL;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::INT, String(), PROPERTY_HINT_INT_IS_POINTER, GDExtensionConstPtr<T>::get_name());
	}
};

template <class T>
struct GetTypeInfo<GDExtensionPtr<T>> {
	static const Variant::Type VARIANT_TYPE = Variant::NIL;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::INT, String(), PROPERTY_HINT_INT_IS_POINTER, GDExtensionPtr<T>::get_name());
	}
};

template <class T>
struct PtrToArg<GDExtensionConstPtr<T>> {
	_FORCE_INLINE_ static GDExtensionConstPtr<T> convert(const void *p_ptr) {
		return GDExtensionConstPtr<T>(reinterpret_cast<const T *>(p_ptr));
	}
	typedef const T *EncodeT;
	_FORCE_INLINE_ static void encode(GDExtensionConstPtr<T> p_val, void *p_ptr) {
		*((const T **)p_ptr) = p_val.data;
	}
};
template <class T>
struct PtrToArg<GDExtensionPtr<T>> {
	_FORCE_INLINE_ static GDExtensionPtr<T> convert(const void *p_ptr) {
		return GDExtensionPtr<T>(reinterpret_cast<const T *>(p_ptr));
	}
	typedef T *EncodeT;
	_FORCE_INLINE_ static void encode(GDExtensionPtr<T> p_val, void *p_ptr) {
		*((T **)p_ptr) = p_val.data;
	}
};

GDVIRTUAL_NATIVE_PTR(void)
GDVIRTUAL_NATIVE_PTR(AudioFrame)
GDVIRTUAL_NATIVE_PTR(bool)
GDVIRTUAL_NATIVE_PTR(char)
GDVIRTUAL_NATIVE_PTR(char16_t)
GDVIRTUAL_NATIVE_PTR(char32_t)
GDVIRTUAL_NATIVE_PTR(wchar_t)
GDVIRTUAL_NATIVE_PTR(uint8_t)
GDVIRTUAL_NATIVE_PTR(uint8_t *)
GDVIRTUAL_NATIVE_PTR(int8_t)
GDVIRTUAL_NATIVE_PTR(uint16_t)
GDVIRTUAL_NATIVE_PTR(int16_t)
GDVIRTUAL_NATIVE_PTR(uint32_t)
GDVIRTUAL_NATIVE_PTR(int32_t)
GDVIRTUAL_NATIVE_PTR(int64_t)
GDVIRTUAL_NATIVE_PTR(uint64_t)
GDVIRTUAL_NATIVE_PTR(float)
GDVIRTUAL_NATIVE_PTR(double)

#endif // NATIVE_PTR_H
