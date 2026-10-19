// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef ANIMATION_LIBRARY_H
#define ANIMATION_LIBRARY_H

#include "core/variant/typed_array.h"
#include "scene/resources/animation.h"

class AnimationLibrary : public Resource {
	GDCLASS(AnimationLibrary, Resource)

	void _set_data(const Dictionary &p_data);
	Dictionary _get_data() const;

	TypedArray<StringName> _get_animation_list() const;

	void _animation_changed(const StringName &p_name);

	friend class AnimationPlayer; //for faster access
	HashMap<StringName, Ref<Animation>> animations;

protected:
	static void _bind_methods();

public:
	static bool is_valid_animation_name(const String &p_name);
	static bool is_valid_library_name(const String &p_name);
	static String validate_library_name(const String &p_name);

	Error add_animation(const StringName &p_name, const Ref<Animation> &p_animation);
	void remove_animation(const StringName &p_name);
	void rename_animation(const StringName &p_name, const StringName &p_new_name);
	bool has_animation(const StringName &p_name) const;
	Ref<Animation> get_animation(const StringName &p_name) const;
	void get_animation_list(List<StringName> *p_animations) const;

	AnimationLibrary();
};

#endif // ANIMATION_LIBRARY_H
