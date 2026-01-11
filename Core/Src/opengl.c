// SPDX-License-Identifier: GPL-3.0-only

#include <stdbool.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cpu.h>

#include "keyboard.h"

#define LCD_FRAMEBUFFER ((uint16_t *)0xC0000000)  // dirección típica SDRAM
#define LCD_WIDTH  480
#define LCD_HEIGHT 272
#define WIDTH 160
#define HEIGHT 144

LTDC_HandleTypeDef hltdc;

int window_width = WIDTH;
int window_height = HEIGHT;

bool fullscreen = false;

Frame buffer;


static void draw_pixel(unsigned char x, unsigned char y, unsigned short color) {

    LCD_FRAMEBUFFER[y * LCD_WIDTH + x] = color;

    /*
    glPointSize(window_width / (float)WIDTH);
    glBegin(GL_POINTS);
    glColor3f((float)(color & 0x1f) / 32, (float)((color >> 5) & 0x1f) / 32, (float)((color >> 10) & 0x1f) / 32);
    glVertex2i(x, y);
    glEnd();
    */
}

static void display() {
	/*
	window_width = glutGet(GLUT_WINDOW_WIDTH);
    window_height = glutGet(GLUT_WINDOW_HEIGHT);

    glLoadIdentity();
    glViewport((window_width - (window_height / HEIGHT) * WIDTH) / 2, 0, ((float)window_height / HEIGHT) * WIDTH,
               window_height);
    glOrtho(0.0f, WIDTH, HEIGHT, 0.0f, 0.0f, 1.0f);

    for (unsigned char y = 0; y < HEIGHT; y++) {
        for (unsigned char x = 0; x < WIDTH; x++) {
            draw_pixel(x, y, buffer.buffer[y][x]);
        }
    }

    glutSwapBuffers();
    */
	//for (unsigned char y = 0; y < HEIGHT; y++) {
	//        for (unsigned char x = 0; x < WIDTH; x++) {
	//        	 uint16_t color = buffer.buffer[y][x];
	//        	 LCD_FRAMEBUFFER[y * LCD_WIDTH + x] = color;
	//        }
	//    }

	    DMA2D_CopyBuffer((uint32_t)buffer.buffer, (uint32_t)LCD_FRAMEBUFFER,
	                     WIDTH, HEIGHT, LCD_WIDTH);

}

static void idle_func() {
    buffer = next_frame();
    display();
}

void display_loop() {
    //int argc = 0;
    //glutInit(&argc, 0);
    //glutInitDisplayMode(GL_DOUBLE);
    //glutInitWindowSize(WIDTH * 4, HEIGHT * 4);
    //glutCreateWindow("cboy");
    //glutDisplayFunc(display);
    //glutIdleFunc(idle_func);
    //glutSpecialFunc(special_key_handler);
    //glutSpecialUpFunc(special_key_up_handler);
    //glutKeyboardFunc(normal_key_handler);
    //glutKeyboardUpFunc(normal_key_up_handler);
    //glutMainLoop();

	while (1){
		buffer = next_frame();
		display();
		poll_buttons();
	}
}

void poll_buttons(void)
{
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) press(RIGHT);
	    else release(RIGHT);

	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) press(LEFT);
	    else release(LEFT);

	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) press(UP);
	    else release(UP);

	    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) press(DOWN);
	    else release(DOWN);

	    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) press(CBOY_KEY_A);
	    else release(CBOY_KEY_A);

	    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET) press(CBOY_KEY_B);
	    else release(CBOY_KEY_B);

	    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET) press(SELECT);
	    else release(SELECT);

	    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET) press(START);
	    else release(START);

}

/*
void toggle_fullscreen() {
    if (!fullscreen) {
        glutFullScreen();
    } else {
        glutPositionWindow(0, 0);
        glutReshapeWindow(WIDTH * 4, HEIGHT * 4);
    }
    fullscreen = !fullscreen;
}
*/
