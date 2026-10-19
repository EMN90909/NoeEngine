// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CPU_PARTICLES_2D_EDITOR_PLUGIN_H
#define CPU_PARTICLES_2D_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/2d/collision_polygon_2d.h"
#include "scene/2d/cpu_particles_2d.h"
#include "scene/gui/box_container.h"

class CheckBox;
class ConfirmationDialog;
class SpinBox;
class EditorFileDialog;
class MenuButton;
class OptionButton;

class CPUParticles2DEditorPlugin : public EditorPlugin {
	GDCLASS(CPUParticles2DEditorPlugin, EditorPlugin);

	enum {
		MENU_LOAD_EMISSION_MASK,
		MENU_CLEAR_EMISSION_MASK,
		MENU_RESTART
	};

	enum EmissionMode {
		EMISSION_MODE_SOLID,
		EMISSION_MODE_BORDER,
		EMISSION_MODE_BORDER_DIRECTED
	};

	CPUParticles2D *particles = nullptr;

	EditorFileDialog *file = nullptr;

	HBoxContainer *toolbar = nullptr;
	MenuButton *menu = nullptr;

	SpinBox *epoints = nullptr;

	ConfirmationDialog *emission_mask = nullptr;
	OptionButton *emission_mask_mode = nullptr;
	CheckBox *emission_colors = nullptr;

	String source_emission_file;

	void _file_selected(const String &p_file);
	void _menu_callback(int p_idx);
	void _generate_emission_mask();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	virtual String get_name() const override { return "CPUParticles2D"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	CPUParticles2DEditorPlugin();
	~CPUParticles2DEditorPlugin();
};

#endif // CPU_PARTICLES_2D_EDITOR_PLUGIN_H
