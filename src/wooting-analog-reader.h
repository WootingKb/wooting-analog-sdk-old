#pragma once

#ifdef WOOTINGANALOGREADER_EXPORTS  
#define WOOTINGANALOGREADER_API __declspec(dllexport)   
#else  
#define WOOTINGANALOGREADER_API __declspec(dllimport)   
#endif

#include "wooting-scan-codes.h"
#include "stdbool.h"
#include "stdint.h"

#define WOOTING_RGB_ROWS 6
#define WOOTING_RGB_COLS 21

typedef void(*void_cb)(void);

/** @brief Check if keyboard connected.

This function offers a quick check if the keyboard is connected. This doesn't open the keyboard or influences reading.

It is recommended to poll this function at the start of your application before starting reading.

@ingroup API

@returns
This function returns true (1) if keyboard is found.
*/
WOOTINGANALOGREADER_API bool wooting_kbd_connected(void);

/** @brief Set callback for when a keyboard disconnect.

The callback will be called when a Wooting keyboard disconnects. Right now this will only trigger after a failed read.

@ingroup API

@returns
None.
*/
WOOTINGANALOGREADER_API void wooting_set_disconnected_cb(void_cb cb);

/** @brief Get the analog value of a single key.

This function returns an analog value of a single key. A SCAN_CODES enumaration is used as argument. These can be found
in ScanCodes.h or on the Wooting developer portal. Scan codes are used to prevent ambiguity between keyboard layouts.

It is not necesarry to initialize the keyboard before reading, but if the keyboard is not connected this function will return 0.

@ingroup API
@param key Scan code of the key that will be read

@returns
This function returns the analog value of the key from 0 - 255
*/
WOOTINGANALOGREADER_API uint8_t wooting_read_analog(uint8_t row, uint8_t column);

/** @brief Get the full analog buffer.

This function can be used to get a buffer of all the keys that are pressed up to a maximum of 16 keys. This can be used for easier access
to all the keys that are currently pressed. The data is ordered as [scan_code0, analog_value0, scan_code1, analog_value1, .... , scan_code15, analog_value15] 

It is not necesarry to initialize the keyboard before reading. If the keyboard is not connected this function will return -1.

@ingroup API
@param data A buffer to put the read data into.
@param items Length of the data buffer

@returns
This function returns items written and -1 on error.
*/
WOOTINGANALOGREADER_API int wooting_read_full_buffer(uint8_t data[], unsigned int length);
