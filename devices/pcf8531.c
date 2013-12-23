/*	Copyright (c) 2013 Adam Dej
	
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

#include <stdlib.h>

#include "pcf8531.h"

#define PCF8531_WIDTH 128
#define PCF8531_HEIGHT 32
#define PCF8531_BANKS 4

bool __pcf8531_set_pixel(uintpix x, uintpix y, void *data) {

	if (x < PCF8531_WIDTH && y < PCF8531_HEIGHT) {
		uint8_t bank = y / 8;
		uint8_t bit = y - (8 * bank);
		((pcf8531_device_data*)data)->framebuffer[x][bank] |= (1 << bit);
		return true;
	}

	return false;
}

bool __pcf8531_unset_pixel(uintpix x, uintpix y, void *data) {

	if (x < PCF8531_WIDTH && y < PCF8531_HEIGHT) {
		uint8_t bank = x / 8;
		uint8_t bit = x - (8 * bank);
		((pcf8531_device_data*)data)->framebuffer[x][bank] &= ~(1 << bit);
		return true;
	}

	return false;
}

bool __pcf8531_clear_display(void *data) {
	uint8_t x, y;
	for (x = 0; x < PCF8531_WIDTH; x++) {
		for(y = 0; y < PCF8531_BANKS; y++) {
			((pcf8531_device_data*)data)->framebuffer[x][y] = 0;
		}
	}
	return true;
}

bool __pcf8531_commit(void *data) {
	if (((pcf8531_device_data*)data)->i2c_send_start() && ((pcf8531_device_data*)data)->i2c_send_address()) {
		bool success = true;
		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b10000000); //Control byte
		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b00000001); //Select function and RAM command page

		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b10000000); //Control byte
		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b00100010); //Select vertical addressing

		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b10000000); //Control byte
		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b01000000); //Y = 0

		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b10000000); //Control byte
		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b10000000); //X = 0

		success &= ((pcf8531_device_data*)data)->i2c_transmit(0b01000000); //Control byte (continuous RAM write)

		uint8_t x, y;

		for (x = 0; x < PCF8531_WIDTH; x++) {
			for(y = 0; y < PCF8531_BANKS; y++) {
				success &= ((pcf8531_device_data*)data)->i2c_transmit(((pcf8531_device_data*)data)->framebuffer[x][y]);
			}
			success &= ((pcf8531_device_data*)data)->i2c_transmit(0b00000000); //We are discarding 4th bank
		}

		((pcf8531_device_data*)data)->i2c_send_stop();

		return success;
	} else {
		((pcf8531_device_data*)data)->i2c_send_stop();
		return false;
	}
}

DISPLAY_1BIT* pcf8531_make_device(bool (*i2c_send_start)(), bool (*i2c_send_address)(),
	bool (*i2c_transmit)(uint8_t data), void (*i2c_send_stop)()) {

	pcf8531_device_data *data = malloc(sizeof(pcf8531_device_data));
	if (data == NULL) return NULL;
	data->i2c_send_start = i2c_send_start;
	data->i2c_send_stop = i2c_send_stop;
	data->i2c_send_address = i2c_send_address;
	data->i2c_transmit = i2c_transmit;

	DISPLAY_1BIT *device = malloc(sizeof(DISPLAY_1BIT));
	if (device == NULL) {
		free(data);
		return NULL;
	}
	device->data = data;
	device->set_pixel = __pcf8531_set_pixel;
	device->unset_pixel = __pcf8531_unset_pixel;
	device->clear_display = __pcf8531_clear_display;
	device->commit = __pcf8531_commit;

	return device;
}