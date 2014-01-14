/*	Copyright (c) 2013, 2014 Adam Dej
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE. */

#ifndef __PCF8531_H
#define __PCF8531_H

#include "../display_device.h"

typedef struct {
	bool (*i2c_send_start)();
	bool (*i2c_send_address)();
	bool (*i2c_transmit)(uint8_t data);
	void (*i2c_send_stop)();
	uint8_t framebuffer[128][4];
} pcf8531_device_data;

DISPLAY_1BIT* pcf8531_make_device(bool (*i2c_send_start)(), bool (*i2c_send_address)(),
	bool (*i2c_transmit)(uint8_t data), void (*i2c_send_stop)());

#endif