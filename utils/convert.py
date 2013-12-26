#!/usr/bin/env python

import sys
from PIL import Image, ImageFont, ImageDraw


def write_in_c(data, name='data'):
	print('const uint8_t {0}[{1}] = {{\n\t'.format(name, len(data)), end="")

	for index, byte in enumerate(data):
		print('{0}'.format(byte), end="")
		if index != len(data)-1:
			print(', ', end="")

		if not index % 10 and index > 0:
			print('\n\t', end="")

	print('};')

def write_in_avr_c(data, name='data'):
	print('const uint8_t {0}[{1}] PROGMEM = {{\n\t'.format(name, len(data)), end="")

	for index, byte in enumerate(data):
		print('{0}'.format(byte), end="")
		if index != len(data)-1:
			print(', ', end="")

		if not index % 10 and index > 0:
			print('\n\t', end="")

	print('};')

def write_in_hex(data):
	print(''.join('0x%02x '%i for i in data))

def write_in_binary(data):
	sys.stdout.buffer.write(bytes(data))

def img_format_0(filename, invert=False):
	image = Image.open(filename).convert('RGBA')
	image = Image.composite(image, Image.new('RGBA', image.size, (255, 255, 255)), image).convert(mode='1')

	pixels = image.load()

	bytes = [0,]
	bytes.extend(image.size)

	byte = 0
	bit = 0

	for y in range(image.size[1]):
		for x in range(image.size[0]):
			byte |= (bool(pixels[x,y]) ^ (not invert)) << bit

			bit += 1
			if bit == 8:
				bytes.append(byte)
				byte = 0
				bit = 0

	return bytes

def font_format_1(filename, size, width=None):

	size += 1
	font = ImageFont.truetype(filename, size+1)
	char_size = [0,size-1]
	chars = list()

	if width is None:
		for code in range(ord('!'), ord('~')+1):
			if char_size[0] < (font.getsize(chr(code))[0]):
				char_size[0] = font.getsize(chr(code))[0]
	else: char_size[0] = width

	bytes = [1]
	bytes.extend(char_size)
	bytes.append(1)

	for code in range(ord('!'), ord('~')+1):
		im = Image.new('RGB', char_size, (255, 255, 255))
		draw = ImageDraw.Draw(im)
		draw.text((0, -1), chr(code), font=font, fill="#000000")
		chars.append(im.convert(mode='1'))

	byte = 0
	bit = 0

	for image in chars:
		pixels = image.load()
		for y in range(image.size[1]):
			for x in range(image.size[0]):
				byte |= (not bool(pixels[x,y])) << bit
				bit += 1
				if bit == 8:
					bytes.append(byte)
					byte = 0
					bit = 0

	return bytes