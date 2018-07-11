/*
* Copyright 2018 Wooting Technologies B.V.
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "wooting-analog-sdk.h"
#include "hidapi.h"
#include "wooting-scan-codes.h"
#include "stdint.h"
#include "stdbool.h"

#define NOKEY 255

#define ANALOG_BUFFER_SIZE 32
#define WOOTING_ONE_VID 0x03EB
#define WOOTING_ONE_PID 0xFF01
#define WOOTING_ONE_ANALOG_USAGE_PAGE 0x1338

static hid_device* keyboard_handle = NULL;
static void_cb disconnected_callback = NULL;
static unsigned char hid_read_buffer[ANALOG_BUFFER_SIZE];

static void wooting_keyboard_disconnected() {
	hid_close(keyboard_handle);
	keyboard_handle = NULL;

	if (disconnected_callback) {
		disconnected_callback();
	}
}

static bool wooting_find_keyboard() {
	struct hid_device_info* hid_info = hid_enumerate(WOOTING_ONE_VID, WOOTING_ONE_PID);
	uint8_t interfaceCount = 0;

	if (hid_info == NULL) {
		return false;
	}

	bool keyboard_found = false;

	// Determine the total amount of interfaces first, because some interfaces can be disabled.
	struct hid_device_info* hid_info_walker = hid_info;
	while (hid_info_walker) {
		interfaceCount++;
		hid_info_walker = hid_info_walker->next;
	}

	// The analog interface is always the last one
	uint8_t interfaceNr = interfaceCount - 1;

	// Reset walker to top and search for the interface number
	hid_info_walker = hid_info;
	while (hid_info_walker) {
		if (hid_info_walker->interface_number == interfaceNr) {
			keyboard_handle = hid_open_path(hid_info_walker->path);
			if (keyboard_handle) {
				keyboard_found = true;
			}
			break;
		}
		
		hid_info_walker = hid_info_walker->next;
	}

	hid_free_enumeration(hid_info);
	return keyboard_found;
}

static bool wooting_refresh_buffer() {
	if (!keyboard_handle) {
		if (!wooting_find_keyboard()) {
			return false;
		}
	}

	int hid_res = hid_read_timeout(keyboard_handle, hid_read_buffer, ANALOG_BUFFER_SIZE, 0);

	// If the read response is -1 the keyboard is disconnected 
	if (hid_res == -1) {
		wooting_keyboard_disconnected();
		return false;
	}
	else {
		return true;
	}
}

void wooting_set_disconnected_cb(void_cb cb) {
	disconnected_callback = cb;
}

bool wooting_kbd_connected() {
	return wooting_find_keyboard();
}

unsigned char wooting_read_analog(uint8_t row, uint8_t column) {
	static uint8_t scan_index_array[WOOTING_RGB_ROWS][WOOTING_RGB_COLS] = {
		{ 0, NOKEY, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 107, 108, 109, 110 },
		{ 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 61, 106, 105, 104, 103 },
		{ 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 62, 102, 101, 100, 99 },
		{ 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 45, 60, NOKEY, NOKEY, NOKEY, 98, 97, 96, NOKEY },
		{ 64, 87, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, NOKEY, 75, NOKEY, 63, NOKEY, 90, 91, 92, 93 },
		{ 80, 81, 82, NOKEY, NOKEY, NOKEY, 83, NOKEY, NOKEY, NOKEY, 84, 85, 86, 79, 76, 77, 78, NOKEY, 95, 94, NOKEY }
	};

	// Just return a 0 value if there's an error. Keyboard will be closed in refresh buffer.
	if (!wooting_refresh_buffer()) {
		return 0;
	}

	// Check if parameters are not out of bounds
	if (row > (WOOTING_RGB_ROWS - 1) || column > (WOOTING_RGB_COLS - 1)) {
		return 0;
	}

	uint8_t scan_index = scan_index_array[row][column];

	for (int i = 1; i < ANALOG_BUFFER_SIZE && hid_read_buffer[i] > 0; i += 2) {
		if (hid_read_buffer[i - 1] == scan_index) {
			// Cap out values to a maximum
			if (hid_read_buffer[i] > 225) {
				return 255;
			}
			else {
				return hid_read_buffer[i];
			}
		}
	}

	// Key not pressed in analog read
	return 0;
}

int wooting_read_full_buffer(uint8_t data[], unsigned int length) {
	int items_written = 0;
	int read_length = length;

	if (!wooting_refresh_buffer()) {
		return -1;
	}

	// Cap elements to read
	if (length > ANALOG_BUFFER_SIZE) {
		read_length = ANALOG_BUFFER_SIZE;
	}

	for (int i = 0; i < read_length; i += 2) {
		uint8_t scan_code = hid_read_buffer[i];
		uint8_t analog_value = hid_read_buffer[i + 1];

		if (analog_value > 0) {
			data[i] = scan_code;

			// Cap out values to a maximum
			if (analog_value > 225) {
				analog_value = 255;
			}
			data[i + 1] = analog_value;
			items_written++;
		}
		else {
			// There will be no other keys once an analog value is 0
			return items_written;
		}
	}

	return items_written;
}
