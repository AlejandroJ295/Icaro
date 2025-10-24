/*
 * controls.c
 *
 *  Created on: Oct 15, 2025
 *      Author: zomeg
 */
#include "gameboy.h"

void press(unsigned char i) { gameboy.controls &= ~(1 << i); }

void release(unsigned char i) { gameboy.controls |= (1 << i); }


