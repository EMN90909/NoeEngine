// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef POST_IMPORT_PLUGIN_SKELETON_TRACK_ORGANIZER_H
#define POST_IMPORT_PLUGIN_SKELETON_TRACK_ORGANIZER_H

#include "resource_importer_scene.h"

class PostImportPluginSkeletonTrackOrganizer : public EditorScenePostImportPlugin {
	GDCLASS(PostImportPluginSkeletonTrackOrganizer, EditorScenePostImportPlugin);

public:
	virtual void get_internal_import_options(InternalImportCategory p_category, List<ResourceImporter::ImportOption> *r_options) override;
	virtual void internal_process(InternalImportCategory p_category, Node *p_base_scene, Node *p_node, Ref<Resource> p_resource, const Dictionary &p_options) override;

	PostImportPluginSkeletonTrackOrganizer();
};

#endif // POST_IMPORT_PLUGIN_SKELETON_TRACK_ORGANIZER_H
