/*
 * gameboy.c
 *
 *  Created on: Oct 13, 2025
 *      Author: zomeg
 */
// SPDX-License-Identifier: GPL-3.0-only

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameboy.h"

static FIL file;
static UINT bytesRead;

Gameboy gameboy = {.controls = 0xFF,
                   .mmu.mbc.rom_bank_number = 1,
                   .mmu.mbc.ram_bank_number = 0,
                   .mmu.mbc.rom_ram_select = 0};

void init() {
    memset(gameboy.mmu.ram, 0, 0x8000);
    memset(gameboy.mmu.bg_palette, 0, 0x1000);
    memset(gameboy.mmu.sprite_palette, 0, 0x1000);
    memset(gameboy.mmu.vram_bank, 0, 0x2000);

    cpu.PC = 0x100;
    cpu.SP = 0xfffe;
    set_AF(0x11b0);
    set_BC(0x13);
    set_DE(0xd8);
    set_HL(0x14d);

    write_mmu(0xFF05, 0x0);
    write_mmu(0xFF06, 0x0);
    write_mmu(0xFF07, 0x0);
    write_mmu(0xFF10, 0x80);
    write_mmu(0xFF11, 0xbf);
    write_mmu(0xFF12, 0xf3);
    write_mmu(0xFF14, 0xbf);
    write_mmu(0xFF16, 0x3f);
    write_mmu(0xFF17, 0x0);
    write_mmu(0xFF19, 0xbf);
    write_mmu(0xFF1a, 0x7f);
    write_mmu(0xFF1b, 0xff);
    write_mmu(0xFF1c, 0x9f);
    write_mmu(0xFF1e, 0xbf);
    write_mmu(0xFF20, 0xff);
    write_mmu(0xFF21, 0x0);
    write_mmu(0xFF22, 0x0);
    write_mmu(0xFF23, 0xbf);
    write_mmu(0xFF24, 0x77);
    write_mmu(0xFF25, 0xf3);
    write_mmu(0xFF26, 0xf1);
    write_mmu(0xFF40, 0x91);
    write_mmu(0xFF42, 0x0);
    write_mmu(0xFF43, 0x0);
    write_mmu(0xFF45, 0x0);
    write_mmu(0xFF47, 0xfc);
    write_mmu(0xFF48, 0xff);
    write_mmu(0xFF49, 0xff);
    write_mmu(0xFF4a, 0x0);
    write_mmu(0xFF4b, 0x0);
    write_mmu(0xFFFF, 0x0);
}

// HAL_SD_ReadBlocks_DMA (SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
void load_rom(char *path) {
	FRESULT res;
	DWORD fileLen;

    printf("ROM path: %s\n", path);

    //char *filename = malloc(strlen(path) + 1);
    //strcpy(filename, path);
    //gameboy.mmu.mbc.filename = filename;

    //FILE *file = fopen(path, "rb");
    res = f_open(&file, path, FA_READ); // Abrir el archivo ROM desde la SD (FatFs)
    if (res != 0) {
        printf("Error reading rom file!");
        exit(1);
    }

    //fseek(file, 0, SEEK_END);
    //unsigned long fileLen = f_size(&file);
    //fseek(file, 0, SEEK_SET);

    DWORD fileLen = f_size(&file); // Obtenemos el tamaño del archivo.

    gameboy.mmu.mbc.rom = malloc(fileLen); // Reserva memoria para la ROM
    if (!gameboy.mmu.mbc.rom) {
    	printf("Error: No hay memoria para ROM\n");
    	f_close(&file);
        return;
    }

    //fread(gameboy.mmu.mbc.rom, fileLen, 1, file);
    f_read(&file, gameboy.mmu.mbc.rom, fileLen, &bytesRead);
    f_close(&file);

    if (res != FR_OK || bytesRead != fileLen) {
    	printf("Error leyendo ROM (codigo %d)\n", res);
    	free(gameboy.mmu.mbc.rom);
    	gameboy.mmu.mbc.rom = NULL;
    	return;
        }

    // Detecta si es CGB (Game Boy Color)
    uint8_t cgb_flag = gameboy.mmu.mbc.rom[0x143];
    gameboy.cgb = (cgb_flag == 0x80 || cgb_flag == 0xC0);

    printf("ROM cargada correctamente (%lu bytes)\n", fileLen);

    init();
}

void load_state() {
    char filename[strlen(gameboy.mmu.mbc.filename) + 5];
    stpcpy(filename, gameboy.mmu.mbc.filename);
    strcat(filename, ".sav");

    //FILE *file = fopen(filename, "rb");
    FRESULT res = f_open(&file, filename, FA_READ);
    if (!file) {
        printf("No state for current rom exists!\n");
        return;
    }

    void *ptr_filename = gameboy.mmu.mbc.filename;
    void *ptr_rom = gameboy.mmu.mbc.rom;

    //read ram state
    //fread(&gameboy.mmu, sizeof(Mmu), 1, file);
    f_read(&file,gameboy.mmu, sizeof Mmu,  &bytesRead);

    // read cpu state
    f_read(&file, &cpu, sizeof(Cpu), &bytesRead);

    gameboy.mmu.mbc.filename = ptr_filename;
    gameboy.mmu.mbc.rom = ptr_rom;

    f_close(&file);
}

void save_state() {
	FRESULT res;
	FIL file;
	UINT bytesWritten;

    // Crear nombre de archivo .sav
	char filename[64]; // tamaño fijo por seguridad, ajusta si necesario
    snprintf(filename, sizeof(filename), "%s.sav", gameboy.mmu.mbc.filename);

    // Abrir archivo para escribir (crear si no existe, sobrescribir si sí)
    res = f_open(&file, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        printf("Error creando archivo de estado (%d)\n", res);
        return;
    };

    // save ram state
    f_write(&file, &gameboy.mmu, sizeof(Mmu), &bytesWritten);

    // save cpu state
    f_write(&file, &cpu, sizeof(Cpu), &bytesWritten);

    f_close(&file);
}


