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
#include <math.h>

#include "1bit.h"

bool draw_line_1bit(uintpix x0, uintpix y0, uintpix x1, uintpix y1, bool color, DISPLAY_1BIT *display) {
	bool (*pixel)(uintpix x, uintpix y, void *data) = color ? display->set_pixel : display->unset_pixel;

	bool success = true;

	int16_t dx = abs(x1-x0);
	int16_t sx = x0<x1 ? 1 : -1;
	int16_t dy = -abs(y1-y0);
	int16_t sy = y0<y1 ? 1 : -1; 
	int16_t err = dx+dy;
	int16_t e2;

	while(1) {
		success &= pixel(x0, y0, display->data);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2*err;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}

	return success;

}

bool draw_rectangle_1bit(uintpix x0, uintpix y0, uintpix x1, uintpix y1, bool color, DISPLAY_1BIT *display) {
	return (draw_line_1bit(x0, y0, x0, y1, color, display) &&
			draw_line_1bit(x1, y0, x1, y1, color, display) &&
			draw_line_1bit(x1, y1, x0, y1, color, display) &&
			draw_line_1bit(x0, y0, x1, y0, color, display));
}

bool fill_rectangle_1bit(uintpix x0, uintpix y0, uintpix x1, uintpix y1, bool color, DISPLAY_1BIT *display) {
	bool success = true;

	for (; x0 <= x1; x0++) {
		success &= draw_line_1bit(x0, y0, x0, y1, color, display);
	}

	return success;
}

bool draw_circle_1bit(uintpix xm, uintpix ym, uintpix r, bool color, DISPLAY_1BIT *display) {
	bool (*pixel)(uintpix x, uintpix y, void *data) = color ? display->set_pixel : display->unset_pixel;

	bool success = true;

	int16_t x = -r;
	int16_t y = 0;
	int16_t err = 2-2*r;
	int16_t r2 = r;

	do {
		success &= pixel(xm-x, ym+y, display->data);
		success &= pixel(xm-y, ym-x, display->data);
		success &= pixel(xm+x, ym-y, display->data);
		success &= pixel(xm+y, ym+x, display->data);
		r2 = err;
		if (r2 <= y) err += ++y*2+1;
		if (r2 > x || err > y) err += ++x*2+1;
	} while (x < 0);

	return success;
}

bool fill_circle_1bit(uintpix xm, uintpix ym, uintpix r, bool color, DISPLAY_1BIT *display) {
	bool success = true;

	int16_t x = -r;
	int16_t y = 0;
	int16_t err = 2-2*r;
	int16_t r2 = r;

	do {
		success &= draw_line_1bit(xm-x, ym+y, xm+x, ym+y, color, display);
		success &= draw_line_1bit(xm-x, ym-y, xm+x, ym-y, color, display);
		r2 = err;
		if (r2 <= y) err += ++y*2+1;
		if (r2 > x || err > y) err += ++x*2+1;
	} while (x < 0);

	return success;
}

bool draw_ellipse_1bit(uintpix x0, uintpix y0, uintpix x1, uintpix y1, bool color, DISPLAY_1BIT *display) {
	bool (*pixel)(uintpix x, uintpix y, void *data) = color ? display->set_pixel : display->unset_pixel;

	bool success = true;

	int32_t a = abs(x1-x0), b = abs(y1-y0), b1 = b&1;
	int32_t dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a;
	int32_t err = dx+dy+b1*a*a, e2;

	if (x0 > x1) { x0 = x1; x1 += a; }
	if (y0 > y1) y0 = y1;
	y0 += (b+1)/2; y1 = y0-b1;
	a *= 8*a; b1 = 8*b*b;

	do {
		success &= pixel(x1, y0, display->data);
		success &= pixel(x0, y0, display->data);
		success &= pixel(x0, y1, display->data);
		success &= pixel(x1, y1, display->data);
		e2 = 2*err;
		if (e2 <= dy) { y0++; y1--; err += dy += a; }
		if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; }
	} while (x0 <= x1);

	while (y0-y1 < b) {
		success &= pixel(x0-1, y0, display->data);
		success &= pixel(x1+1, y0++, display->data); 
		success &= pixel(x0-1, y1, display->data);
		success &= pixel(x1+1, y1--, display->data); 
	}

	return success;
}

bool fill_ellipse_1bit(uintpix x0, uintpix y0, uintpix x1, uintpix y1, bool color, DISPLAY_1BIT *display) {
	bool success = true;

	int32_t a = abs(x1-x0), b = abs(y1-y0), b1 = b&1;
	int32_t dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a;
	int32_t err = dx+dy+b1*a*a, e2;

	if (x0 > x1) { x0 = x1; x1 += a; }
	if (y0 > y1) y0 = y1;
	y0 += (b+1)/2; y1 = y0-b1;
	a *= 8*a; b1 = 8*b*b;

	do {
		success &= draw_line_1bit(x1, y0, x0, y0, color, display);
		success &= draw_line_1bit(x0, y1, x1, y1, color, display);
		e2 = 2*err;
		if (e2 <= dy) { y0++; y1--; err += dy += a; }
		if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; }
	} while (x0 <= x1);

	return success;
}

bool draw_triangle_1bit(uintpix x0, uintpix y0, uintpix x1, uintpix y1, uintpix x2, uintpix y2, bool color, DISPLAY_1BIT *display) {
	return (draw_line_1bit(x0, y0, x1, y1, color, display) &&
			draw_line_1bit(x1, y1, x2, y2, color, display) &&
			draw_line_1bit(x2, y2, x0, y0, color, display));
}

uintpix min(uintpix a, uintpix b, uintpix c) {
	if (b < a) a = b;
	if (c < a) a = c;
	return a;
}

uintpix max(uintpix a, uintpix b, uintpix c) {
	if (b > a) a = b;
	if (c > a) a = c;
	return a;
}

bool fill_triangle_1bit(uintpix x0, uintpix y0, uintpix x1, uintpix y1, uintpix x2, uintpix y2, bool color, DISPLAY_1BIT *display) {
	
	bool (*pixel)(uintpix x, uintpix y, void *data) = color ? display->set_pixel : display->unset_pixel;

	uintpix minx = min(x0, x1, x2);
    uintpix maxx = max(x0, x1, x2);
    uintpix miny = min(y0, y1, y2);
    uintpix maxy = max(y0, y1, y2);

    uintpix x, y;
    bool success = true;

    for (x = minx; x <= maxx; x++) {
    	for (y = miny; y <= maxy; y++) {
    		if((x0 - x1) * (y - y0) - (y0 - y1) * (x - x0) > 0 &&
            	(x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) > 0 &&
            	(x2 - x0) * (y - y2) - (y2 - y0) * (x - x2) > 0) success &= pixel(x, y, display->data);
    	}
    }

    return success;

}

bool __draw_bitmap_f0(uintpix x0, uintpix y0, void *address, uint8_t(*read_byte)(void* address), DISPLAY_1BIT *display) {

	uint8_t x, y;
 	uint8_t maxX = read_byte(address+1);
 	uint8_t maxY = read_byte(address+2);
 	uint8_t bit = 0;
 	uint16_t byte = 0;

 	bool success = true;

 	for (y = 0; y < maxY; y++) {
 		for (x = 0; x < maxX; x++) {
 			if (read_byte(address+3+byte) & (1 << bit))
 				success &= display->set_pixel(x+x0, y+y0, display->data);
 			bit++;
 			if (bit == 8) {
 				bit = 0;
 				byte++;
 			}
 		}
 	}

 	return success;

}

bool draw_bitmap_1bit(uintpix x0, uintpix y0, void *address, uint8_t(*read_byte)(void *address), DISPLAY_1BIT *display) {

	uint8_t format = read_byte(address);

	switch(format) {
		case 0: return __draw_bitmap_f0(x0, y0, address, read_byte, display);
		default: return false; //Unsupported format

	}
}

bool __draw_text_f1(uintpix x0, uintpix y0, char *string, void *font, uint8_t(*read_byte)(void *address), DISPLAY_1BIT *display) {

	uint8_t x, y;
 	uint8_t offset = 0;

 	uint8_t sizeX = read_byte(font+1);
 	uint8_t sizeY = read_byte(font+2);
 	uint8_t offsetInc = sizeX + read_byte(font+3);

 	while (*string) {

 		if (*string >= '!' && *string <= '~') {
 			for (y = 0; y < sizeY; y++) {
 				for (x = 0; x < sizeX; x++) {
 					uint16_t bit = ((sizeX*sizeY)*(*string - '!'))+(x+(y*sizeX));
 					uint8_t byte = read_byte(font+4+(bit/8));
 					if (byte & (1 << bit%8)) display->set_pixel(x0+offset+x, y0+y, display->data);
 				}
 			}
 		}

 		offset += offsetInc;
 		string++;
 	}

}

bool draw_text_1bit(uintpix x0, uintpix y0, char *string, void *font, uint8_t(*read_byte)(void *address), DISPLAY_1BIT *display) {

	uint8_t format = read_byte(font);

	switch(format) {
		case 1: return __draw_text_f1(x0, y0, string, font, read_byte, display);
		default: return false; //Unsupported format
	}
}