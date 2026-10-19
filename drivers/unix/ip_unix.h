// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef IP_UNIX_H
#define IP_UNIX_H

#include "core/io/ip.h"

#if defined(UNIX_ENABLED) || defined(WINDOWS_ENABLED)

class IPUnix : public IP {
	GDCLASS(IPUnix, IP);

	virtual void _resolve_hostname(List<IPAddress> &r_addresses, const String &p_hostname, Type p_type = TYPE_ANY) const override;

	static IP *_create_unix();

public:
	virtual void get_local_interfaces(HashMap<String, Interface_Info> *r_interfaces) const override;

	static void make_default();
	IPUnix();
};

#endif

#endif // IP_UNIX_H
