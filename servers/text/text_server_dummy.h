// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef TEXT_SERVER_DUMMY_H
#define TEXT_SERVER_DUMMY_H

#include "servers/text/text_server_extension.h"

/*************************************************************************/

class TextServerDummy : public TextServerExtension {
	GDCLASS(TextServerDummy, TextServerExtension);
	_THREAD_SAFE_CLASS_

public:
	virtual String get_name() const override {
		return "Dummy";
	}
};

#endif // TEXT_SERVER_DUMMY_H
