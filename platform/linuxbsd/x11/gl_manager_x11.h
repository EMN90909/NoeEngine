// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef GL_MANAGER_X11_H
#define GL_MANAGER_X11_H

#ifdef X11_ENABLED

#ifdef GLES3_ENABLED

#include "core/os/os.h"
#include "core/templates/local_vector.h"

#ifdef SOWRAP_ENABLED
#include "dynwrappers/xlib-so_wrap.h"

#include "dynwrappers/xext-so_wrap.h"
#include "dynwrappers/xrender-so_wrap.h"
#else
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/extensions/Xext.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/shape.h>
#endif

#include "servers/display_server.h"

struct GLManager_X11_Private;

class GLManager_X11 {
public:
	enum ContextType {
		GLES_3_0_COMPATIBLE,
	};

private:
	// any data specific to the window
	struct GLWindow {
		bool in_use = false;

		// the external ID .. should match the GL window number .. unused I think
		DisplayServer::WindowID window_id = DisplayServer::INVALID_WINDOW_ID;
		int width = 0;
		int height = 0;
		::Window x11_window;
		int gldisplay_id = 0;
	};

	struct GLDisplay {
		GLDisplay() { context = nullptr; }
		~GLDisplay();
		GLManager_X11_Private *context = nullptr;
		::Display *x11_display;
		XVisualInfo x_vi;
	};

	// just for convenience, window and display struct
	struct XWinDisp {
		::Window x11_window;
		::Display *x11_display;
	} _x_windisp;

	LocalVector<GLWindow> _windows;
	LocalVector<GLDisplay> _displays;

	GLWindow *_current_window = nullptr;

	void _internal_set_current_window(GLWindow *p_win);

	GLWindow &get_window(unsigned int id) { return _windows[id]; }
	const GLWindow &get_window(unsigned int id) const { return _windows[id]; }

	const GLDisplay &get_current_display() const { return _displays[_current_window->gldisplay_id]; }
	const GLDisplay &get_display(unsigned int id) { return _displays[id]; }

	bool double_buffer;
	bool direct_render;
	int glx_minor, glx_major;
	bool use_vsync;
	ContextType context_type;

private:
	int _find_or_create_display(Display *p_x11_display);
	Error _create_context(GLDisplay &gl_display);

public:
	XVisualInfo get_vi(Display *p_display, Error &r_error);
	Error window_create(DisplayServer::WindowID p_window_id, ::Window p_window, Display *p_display, int p_width, int p_height);
	void window_destroy(DisplayServer::WindowID p_window_id);
	void window_resize(DisplayServer::WindowID p_window_id, int p_width, int p_height);

	void release_current();
	void make_current();
	void swap_buffers();

	void window_make_current(DisplayServer::WindowID p_window_id);

	Error initialize(Display *p_display);

	void set_use_vsync(bool p_use);
	bool is_using_vsync() const;

	void *get_glx_context(DisplayServer::WindowID p_window_id);

	GLManager_X11(const Vector2i &p_size, ContextType p_context_type);
	~GLManager_X11();
};

#endif // GLES3_ENABLED
#endif // X11_ENABLED

#endif // GL_MANAGER_X11_H
