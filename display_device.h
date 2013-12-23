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

#ifndef __DISPLAY_DEVICE_H
#define __DISPLAY_DEVICE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef EMBDEGFX_16
	typedef uint16_t uintpix;
#else
	typedef uint8_t uintpix;
#endif

typedef struct {
	void *data;
	bool (*set_pixel)(uintpix x, uintpix y, void *data);
	bool (*unset_pixel)(uintpix x, uintpix y, void *data);
	bool (*clear_display)(void *data);
	bool (*commit)(void *data);
} DISPLAY_1BIT;

typedef struct {
	//TODO
} DISPLAY_8BIT;

typedef struct {
	//TODO
} DISPLAY_24BIT;

//If none of those modes is suitable feel free to add your own

#endif