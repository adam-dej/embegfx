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

#include "virtual_mirror.h"

bool __mirror_set_pixel(uintpix x, uintpix y, void *data) {
	return ((mirror_data*)data)->display->set_pixel(
		(((mirror_data*)data)->mode & 1) ? (((mirror_data*)data)->x - x) : x,
		(((mirror_data*)data)->mode & 2) ? (((mirror_data*)data)->y - y) : y,
		((mirror_data*)data)->display->data);
}

bool __mirror_unset_pixel(uintpix x, uintpix y, void *data) {
	return ((mirror_data*)data)->display->unset_pixel(
		(((mirror_data*)data)->mode & 1) ? (((mirror_data*)data)->x - x) : x,
		(((mirror_data*)data)->mode & 2) ? (((mirror_data*)data)->y - y) : y,
		((mirror_data*)data)->display->data);
}

bool __mirror_clear_display(void *data) {
	return ((mirror_data*)data)->display->clear_display(((mirror_data*)data)->display->data);
}

bool __mirror_commit(void *data) {
	return ((mirror_data*)data)->display->commit(((mirror_data*)data)->display->data);
}

DISPLAY_1BIT* mirror_device(DISPLAY_1BIT *display, uint8_t mode, uintpix x, uintpix y) {
	mirror_data *data = malloc(sizeof(mirror_data));
	if (data == NULL) return NULL;
	data->display = display;
	data->mode = mode;
	data->x = x-1;
	data->y = y-1;

	DISPLAY_1BIT *device = malloc(sizeof(DISPLAY_1BIT));
	if (device == NULL) {
		free(data);
		return NULL;
	}
	device->set_pixel = __mirror_set_pixel;
	device->unset_pixel = __mirror_unset_pixel;
	device->clear_display = __mirror_clear_display;
	device->commit = __mirror_commit;
	device->data = data;

	return device;
}