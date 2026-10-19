// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef CAMERA_SERVER_H
#define CAMERA_SERVER_H

#include "core/object/class_db.h"
#include "core/object/ref_counted.h"
#include "core/os/thread_safe.h"
#include "core/templates/rid.h"
#include "core/variant/variant.h"

/**
	The camera server is a singleton object that gives access to the various
	camera feeds that can be used as the background for our environment.
**/

class CameraFeed;
template <typename T>
class TypedArray;

class CameraServer : public Object {
	GDCLASS(CameraServer, Object);
	_THREAD_SAFE_CLASS_

public:
	enum FeedImage {
		FEED_RGBA_IMAGE = 0,
		FEED_YCBCR_IMAGE = 0,
		FEED_Y_IMAGE = 0,
		FEED_CBCR_IMAGE = 1,
		FEED_IMAGES = 2
	};

	typedef CameraServer *(*CreateFunc)();

private:
protected:
	static CreateFunc create_func;

	Vector<Ref<CameraFeed>> feeds;

	static CameraServer *singleton;

	static void _bind_methods();

	template <class T>
	static CameraServer *_create_builtin() {
		return memnew(T);
	}

public:
	static CameraServer *get_singleton();

	template <class T>
	static void make_default() {
		create_func = _create_builtin<T>;
	}

	static CameraServer *create() {
		CameraServer *server = create_func ? create_func() : memnew(CameraServer);
		return server;
	};

	// Right now we identify our feed by it's ID when it's used in the background.
	// May see if we can change this to purely relying on CameraFeed objects or by name.
	int get_free_id();
	int get_feed_index(int p_id);
	Ref<CameraFeed> get_feed_by_id(int p_id);

	// Add and remove feeds.
	void add_feed(const Ref<CameraFeed> &p_feed);
	void remove_feed(const Ref<CameraFeed> &p_feed);

	// Get our feeds.
	Ref<CameraFeed> get_feed(int p_index);
	int get_feed_count();
	TypedArray<CameraFeed> get_feeds();

	// Intended for use with custom CameraServer implementation.
	RID feed_texture(int p_id, FeedImage p_texture);

	CameraServer();
	~CameraServer();
};

VARIANT_ENUM_CAST(CameraServer::FeedImage);

#endif // CAMERA_SERVER_H
