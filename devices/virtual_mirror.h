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

#ifndef __VIRTUAL_MIRROR_H
#define __VIRTUAL_MIRROR_H

#include "../display_device.h"

#define VIRTUAL_MIRROR_X 1
#define VIRTUAL_MIRROR_Y 2

typedef struct {
	DISPLAY_1BIT *display;
	uint8_t mode;
	uintpix x;
	uintpix y;
} mirror_data;

DISPLAY_1BIT* mirror_device(DISPLAY_1BIT *display, uint8_t mode, uintpix x, uintpix y);

#endif