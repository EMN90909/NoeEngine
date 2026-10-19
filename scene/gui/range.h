// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef RANGE_H
#define RANGE_H

#include "scene/gui/control.h"

class Range : public Control {
	GDCLASS(Range, Control);

	struct Shared {
		double val = 0.0;
		double min = 0.0;
		double max = 100.0;
		double step = 1.0;
		double page = 0.0;
		bool exp_ratio = false;
		bool allow_greater = false;
		bool allow_lesser = false;
		HashSet<Range *> owners;
		void emit_value_changed();
		void emit_changed(const char *p_what = "");
		void redraw_owners();
	};

	Shared *shared = nullptr;

	void _ref_shared(Shared *p_shared);
	void _unref_shared();

	void _share(Node *p_range);

	void _value_changed_notify();
	void _changed_notify(const char *p_what = "");
	void _set_value_no_signal(double p_val);

protected:
	virtual void _value_changed(double p_value);

	static void _bind_methods();

	bool _rounded_values = false;

	GDVIRTUAL1(_value_changed, double)

public:
	void set_value(double p_val);
	void set_value_no_signal(double p_val);
	void set_min(double p_min);
	void set_max(double p_max);
	void set_step(double p_step);
	void set_page(double p_page);
	void set_as_ratio(double p_value);

	double get_value() const;
	double get_min() const;
	double get_max() const;
	double get_step() const;
	double get_page() const;
	double get_as_ratio() const;

	void set_use_rounded_values(bool p_enable);
	bool is_using_rounded_values() const;

	void set_exp_ratio(bool p_enable);
	bool is_ratio_exp() const;

	void set_allow_greater(bool p_allow);
	bool is_greater_allowed() const;

	void set_allow_lesser(bool p_allow);
	bool is_lesser_allowed() const;

	void share(Range *p_range);
	void unshare();

	PackedStringArray get_configuration_warnings() const override;

	Range();
	~Range();
};

#endif // RANGE_H
