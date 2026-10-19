// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_PREVIEW_PLUGINS_H
#define EDITOR_PREVIEW_PLUGINS_H

#include "core/templates/safe_refcount.h"
#include "editor/editor_resource_preview.h"

void post_process_preview(Ref<Image> p_image);

class EditorTexturePreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorTexturePreviewPlugin, EditorResourcePreviewGenerator);

public:
	virtual bool handles(const String &p_type) const override;
	virtual bool generate_small_preview_automatically() const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorTexturePreviewPlugin();
};

class EditorImagePreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorImagePreviewPlugin, EditorResourcePreviewGenerator);

public:
	virtual bool handles(const String &p_type) const override;
	virtual bool generate_small_preview_automatically() const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorImagePreviewPlugin();
};

class EditorBitmapPreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorBitmapPreviewPlugin, EditorResourcePreviewGenerator);

public:
	virtual bool handles(const String &p_type) const override;
	virtual bool generate_small_preview_automatically() const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorBitmapPreviewPlugin();
};

class EditorPackedScenePreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorPackedScenePreviewPlugin, EditorResourcePreviewGenerator);

public:
	virtual bool handles(const String &p_type) const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;
	virtual Ref<Texture2D> generate_from_path(const String &p_path, const Size2 &p_size) const override;

	EditorPackedScenePreviewPlugin();
};

class EditorMaterialPreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorMaterialPreviewPlugin, EditorResourcePreviewGenerator);

	RID scenario;
	RID sphere;
	RID sphere_instance;
	RID viewport;
	RID viewport_texture;
	RID light;
	RID light_instance;
	RID light2;
	RID light_instance2;
	RID camera;
	RID camera_attributes;
	Semaphore preview_done;

	void _generate_frame_started();
	void _preview_done();

public:
	virtual bool handles(const String &p_type) const override;
	virtual bool generate_small_preview_automatically() const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorMaterialPreviewPlugin();
	~EditorMaterialPreviewPlugin();
};

class EditorScriptPreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorScriptPreviewPlugin, EditorResourcePreviewGenerator);

public:
	virtual bool handles(const String &p_type) const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorScriptPreviewPlugin();
};

class EditorAudioStreamPreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorAudioStreamPreviewPlugin, EditorResourcePreviewGenerator);

public:
	virtual bool handles(const String &p_type) const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorAudioStreamPreviewPlugin();
};

class EditorMeshPreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorMeshPreviewPlugin, EditorResourcePreviewGenerator);

	RID scenario;
	RID mesh_instance;
	RID viewport;
	RID viewport_texture;
	RID light;
	RID light_instance;
	RID light2;
	RID light_instance2;
	RID camera;
	RID camera_attributes;
	Semaphore preview_done;

	void _generate_frame_started();
	void _preview_done();

public:
	virtual bool handles(const String &p_type) const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorMeshPreviewPlugin();
	~EditorMeshPreviewPlugin();
};

class EditorFontPreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorFontPreviewPlugin, EditorResourcePreviewGenerator);

	RID viewport;
	RID viewport_texture;
	RID canvas;
	RID canvas_item;
	Semaphore preview_done;

	void _generate_frame_started();
	void _preview_done();

public:
	virtual bool handles(const String &p_type) const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;
	virtual Ref<Texture2D> generate_from_path(const String &p_path, const Size2 &p_size) const override;

	EditorFontPreviewPlugin();
	~EditorFontPreviewPlugin();
};

class EditorTileMapPatternPreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorTileMapPatternPreviewPlugin, EditorResourcePreviewGenerator);

	Semaphore preview_done;

	void _generate_frame_started();
	void _preview_done();

public:
	virtual bool handles(const String &p_type) const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorTileMapPatternPreviewPlugin();
	~EditorTileMapPatternPreviewPlugin();
};

class EditorGradientPreviewPlugin : public EditorResourcePreviewGenerator {
	GDCLASS(EditorGradientPreviewPlugin, EditorResourcePreviewGenerator);

public:
	virtual bool handles(const String &p_type) const override;
	virtual bool generate_small_preview_automatically() const override;
	virtual Ref<Texture2D> generate(const Ref<Resource> &p_from, const Size2 &p_size) const override;

	EditorGradientPreviewPlugin();
};

#endif // EDITOR_PREVIEW_PLUGINS_H
