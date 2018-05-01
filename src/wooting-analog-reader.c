#include "wooting-analog-reader.h"
#include "hidapi.h"
#include "wooting-scan-codes.h"

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

static int wooting_find_keyboard() {
	struct hid_device_info* hid_info = hid_enumerate(WOOTING_ONE_VID, WOOTING_ONE_PID);

	if (hid_info == NULL) {
		return -1;
	}

	// Loop through linked list of hid_info untill the analog interface is found
	while (hid_info->next) {
		if (hid_info->usage_page == WOOTING_ONE_ANALOG_USAGE_PAGE) {
			keyboard_handle = hid_open_path(hid_info->path);
			return 0;
		}
		else {
			hid_info = hid_info->next;
		}
	}

	// Analog interface not found
	return -1;
}

static int wooting_refresh_buffer() {
	if (!keyboard_handle) {
		return wooting_find_keyboard();
	}

	int hid_res = hid_read_timeout(keyboard_handle, hid_read_buffer, ANALOG_BUFFER_SIZE, 0);

	// If the read response is -1 the keyboard is disconnected 
	if (hid_res == -1) {
		wooting_keyboard_disconnected();
	}

	return hid_res;
}

void wooting_set_disconnected_cb(void_cb cb) {
	disconnected_callback = cb;
}

int wooting_kbd_connected() {
	return hid_enumerate(WOOTING_ONE_VID, WOOTING_ONE_PID) == NULL ? -1 : 0;
}

unsigned char wooting_read_analog(SCAN_CODES scan_index) {
	int hid_res = wooting_refresh_buffer();

	// Just return a 0 value if there's an error. Keyboard will be closed in refresh buffer.
	if (hid_res == -1) {
		return 0;
	}

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

int wooting_read_full_buffer(wooting_full_buffer data[], unsigned int length) {
	int items_written = 0;
	int items_to_read = length;
	int hid_res = wooting_refresh_buffer();

	if (hid_res == -1) {
		return -1;
	}

	// Cap elements to read
	if (length > ANALOG_BUFFER_SIZE / 2) {
		items_to_read = ANALOG_BUFFER_SIZE;
	}

	for (int i = 0; i < items_to_read; i++) {
		unsigned char scan_code = hid_read_buffer[i * 2];
		unsigned char analog_value = hid_read_buffer[i * 2 + 1];

		if (analog_value > 0) {
			data[i].analog_value = analog_value;
			data[i].scan_code = scan_code;
			items_written++;
		}
		else {
			data[i].analog_value = 0;
			data[i].scan_code = SCAN_NO_KEY;
		}
	}

	return items_written;
}
