#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "grlib.h"

static const uint32_t palette[] =
{
		0xffffff, 0x000000
};

static const uint8_t Alien_Pixel_Data[] = {
		0xff,
		0xff,
		0xff,
		0b11110001,
		0b11100111,
		0b01000001,
		0b10010010,
		0b11000010,
		0b11000011,
		0b11000010,
		0b10010010,
		0b01000001,
		0b11100111,
		0b11110001,
        0xff,
        0xff,
};

static const uint8_t Bullet_Pixel_Data[] = {
       0xff,
       0xff,
       0xff,
       0xff,
       0xff,
       0b11110000,
       0b11000000,
       0b00000011,
       0b00000011,
       0b11000000,
       0b11110000,
       0xff,
       0xff,
       0xff,
       0xff,
       0xff,
};

static Graphics_Image Alien_Sprite = {
		IMAGE_FMT_1BPP_UNCOMP,
		8,
		16,
		2,
		palette,
		Alien_Pixel_Data
};

static Graphics_Image Bullet_Sprite = {
       IMAGE_FMT_1BPP_UNCOMP,
       8,
       16,
       2,
       palette,
       Bullet_Pixel_Data
};

#endif
