// SPDX-License-Identifier: MIT
// Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).
// Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.
// License and third-party attribution: LICENSE.txt, COPYRIGHT.txt, NOTICE.md

#ifndef MIDI_DRIVER_H
#define MIDI_DRIVER_H

#include "core/typedefs.h"
#include "core/variant/variant.h"

/**
 * Multi-Platform abstraction for accessing to MIDI.
 */

class MIDIDriver {
	static MIDIDriver *singleton;
	static uint8_t last_received_message;

public:
	static MIDIDriver *get_singleton();
	void set_singleton();

	virtual Error open() = 0;
	virtual void close() = 0;

	virtual PackedStringArray get_connected_inputs();

	static void receive_input_packet(uint64_t timestamp, uint8_t *data, uint32_t length);

	MIDIDriver();
	virtual ~MIDIDriver() {}
};

#endif // MIDI_DRIVER_H
