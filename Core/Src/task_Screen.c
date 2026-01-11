/*
 * task_Screen.c
 *
 *  Created on: Dic 29, 2025
 *      Author: zomeg
 */

#include "task_Screen.h"

#define LCD_FRAMEBUFFER ((uint16_t *)0xC0000000)  // dirección típica SDRAM
#define LCD_WIDTH  480
#define LCD_HEIGHT 272
#define WIDTH 160
#define HEIGHT 144

LTDC_HandleTypeDef hltdc;

TaskHandle_t screenTaskHandle;
int window_width = WIDTH;
int window_height = HEIGHT;

bool fullscreen = false;

Frame buffer;

void controlsScreenTask(void * pargs){

	while (1)
	{

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		poll_buttons();

		buffer = next_frame();
		display();

	}
}

void createScreenTask(){
	xTaskCreate(controlsScreenTask, "controlsScreenTask", 256, NULL, 2, &screenTaskHandle);

}


static void draw_pixel(unsigned char x, unsigned char y, unsigned short color) {

    LCD_FRAMEBUFFER[y * LCD_WIDTH + x] = color;
}

static void display() {
	    DMA2D_CopyBuffer((uint32_t)buffer.buffer, (uint32_t)LCD_FRAMEBUFFER,
	                     WIDTH, HEIGHT, LCD_WIDTH);
}

static void idle_func() {
    buffer = next_frame();
    display();
}
