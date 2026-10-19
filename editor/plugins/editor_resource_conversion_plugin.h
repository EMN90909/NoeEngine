// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef EDITOR_RESOURCE_CONVERSION_PLUGIN_H
#define EDITOR_RESOURCE_CONVERSION_PLUGIN_H

#include "core/io/resource.h"
#include "core/object/gdvirtual.gen.inc"
#include "core/object/script_language.h"

class EditorResourceConversionPlugin : public RefCounted {
	GDCLASS(EditorResourceConversionPlugin, RefCounted);

protected:
	static void _bind_methods();

	GDVIRTUAL0RC(String, _converts_to)
	GDVIRTUAL1RC(bool, _handles, Ref<Resource>)
	GDVIRTUAL1RC(Ref<Resource>, _convert, Ref<Resource>)

public:
	virtual String converts_to() const;
	virtual bool handles(const Ref<Resource> &p_resource) const;
	virtual Ref<Resource> convert(const Ref<Resource> &p_resource) const;
};

#endif // EDITOR_RESOURCE_CONVERSION_PLUGIN_H
