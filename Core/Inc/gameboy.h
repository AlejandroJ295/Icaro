/*
 * gameboy.h
 *
 *  Created on: Oct 13, 2025
 *      Author: zomeg
 */

#ifndef INC_GAMEBOY_H_
#define INC_GAMEBOY_H_


#include "cpu.h"
#include "mmu.h"

typedef struct {
    Mmu mmu;
    unsigned char controls;
    Frame framebuffer;
    bool cgb;
} Gameboy;

extern Gameboy gameboy;

void serial_print(char c);

void load_rom(char *path);

void load_state();

void save_state();

#ifdef __cplusplus
}
#endif

#endif // LIBCBOY_GAMEBOY_H

#endif /* INC_GAMEBOY_H_ */
