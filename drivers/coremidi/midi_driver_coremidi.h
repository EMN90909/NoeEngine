// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MIDI_DRIVER_COREMIDI_H
#define MIDI_DRIVER_COREMIDI_H

#ifdef COREMIDI_ENABLED

#include "core/os/midi_driver.h"
#include "core/templates/vector.h"

#import <CoreMIDI/CoreMIDI.h>
#include <stdio.h>

class MIDIDriverCoreMidi : public MIDIDriver {
	MIDIClientRef client = 0;
	MIDIPortRef port_in;

	Vector<MIDIEndpointRef> connected_sources;

	static void read(const MIDIPacketList *packet_list, void *read_proc_ref_con, void *src_conn_ref_con);

public:
	virtual Error open();
	virtual void close();

	PackedStringArray get_connected_inputs();

	MIDIDriverCoreMidi();
	virtual ~MIDIDriverCoreMidi();
};

#endif // COREMIDI_ENABLED

#endif // MIDI_DRIVER_COREMIDI_H
