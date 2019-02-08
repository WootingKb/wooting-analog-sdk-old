/*
* Copyright 2018 Wooting Technologies B.V.
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WOOTINGANALOGSDK_EXPORTS  
#define WOOTINGANALOGSDK_API __declspec(dllexport)   
#else  
#define WOOTINGANALOGSDK_API __declspec(dllimport)   
#endif

#include "stdbool.h"
#include "stdint.h"

#define WOOTING_RGB_ROWS 6
#define WOOTING_RGB_COLS 21

typedef void(*void_cb)(void);

/** @brief Check if keyboard connected.

This function offers a check if the keyboard is connected. 
The read functions will automatically call this function on first use.

@ingroup API

@returns
This function returns true (1) if keyboard is found.
*/
WOOTINGANALOGSDK_API bool wooting_kbd_connected(void);

/** @brief Set callback for when a keyboard disconnect.

The callback will be called when a Wooting keyboard disconnects. It will only trigger after a failed read.

@ingroup API
@param cb The function pointer of the callback

@returns
None.
*/
WOOTINGANALOGSDK_API void wooting_set_disconnected_cb(void_cb cb);

/** @brief Get the analog value of a single key.

This function returns an analog value of a single key. The arguments are a row and column index of the keyboard array.
The keyboard matrix with row columns indexing can be found in the resources folder.

It is not necessary to initialize the keyboard before reading. If the keyboard is not connected this function will return 0.
Indexes out of bound will also return 0.

@ingroup API
@param row Row index of the key that will be read
@param column Column index of the key that will be read

@returns
This function returns the analog value of the key from 0 - 255
*/
WOOTINGANALOGSDK_API uint8_t wooting_read_analog(uint8_t row, uint8_t column);

/** @brief Get the full analog buffer.

This function can be used to get a buffer of all the keys that are pressed up to a maximum of 16 keys. The maximum size of the buffer is 32 bytes (2 bytes * 16 keys)
The data is ordered as [scan_code0, analog_value0, scan_code1, analog_value1, .... , scan_code15, analog_value15] 
The keyboard matrix with scan code indexing can be found in the resources folder.

It is not necessary to initialize the keyboard before reading. If the keyboard is not connected this function will return -1.

@ingroup API
@param data A buffer to put the read data into (2 - 32 bytes).
@param items Length of the data buffer in bytes

@returns
This function returns items written and -1 on error.
*/
WOOTINGANALOGSDK_API int wooting_read_full_buffer(uint8_t data[], unsigned int length);

#ifdef __cplusplus
}
#endif
