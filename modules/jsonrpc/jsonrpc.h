// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef JSONRPC_H
#define JSONRPC_H

#include "core/object/class_db.h"
#include "core/variant/variant.h"

class JSONRPC : public Object {
	GDCLASS(JSONRPC, Object)

	HashMap<String, Object *> method_scopes;

protected:
	static void _bind_methods();

public:
	JSONRPC();
	~JSONRPC();

	enum ErrorCode {
		PARSE_ERROR = -32700,
		INVALID_REQUEST = -32600,
		METHOD_NOT_FOUND = -32601,
		INVALID_PARAMS = -32602,
		INTERNAL_ERROR = -32603,
	};

	Dictionary make_response_error(int p_code, const String &p_message, const Variant &p_id = Variant()) const;
	Dictionary make_response(const Variant &p_value, const Variant &p_id);
	Dictionary make_notification(const String &p_method, const Variant &p_params);
	Dictionary make_request(const String &p_method, const Variant &p_params, const Variant &p_id);

	Variant process_action(const Variant &p_action, bool p_process_arr_elements = false);
	String process_string(const String &p_input);

	void set_scope(const String &p_scope, Object *p_obj);
};

VARIANT_ENUM_CAST(JSONRPC::ErrorCode);

#endif // JSONRPC_H
