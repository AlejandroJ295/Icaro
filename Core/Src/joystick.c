/*
 * joystick.c
 *
 *  Created on: Oct 15, 2025
 *      Author: zomeg
 */

#include "joystick.h"
extern TaskHandle_t screenTaskHandle

void controlsJoystickTask(void * pargs)
{
    while (1)
    {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
        {
            press(RIGHT);
            xTaskNotifyGive(screenTaskHandle);
        }
        else
        {
            release(RIGHT);
        }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
        {
            press(LEFT);
            xTaskNotifyGive(screenTaskHandle);
        }
        else
        {
            release(LEFT);
        }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
        {
            press(UP);
            xTaskNotifyGive(screenTaskHandle);
        }
        else
        {
            release(UP);
        }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET)
        {
            press(DOWN);
            xTaskNotifyGive(screenTaskHandle);
        }
        else
        {
            release(DOWN);
        }

        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
        {
            press(CBOY_KEY_A);
            xTaskNotifyGive(screenTaskHandle);
        }
        else
        {
            release(CBOY_KEY_A);
        }

        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
        {
            press(CBOY_KEY_B);
            xTaskNotifyGive(screenTaskHandle);
        }
        else
        {
            release(CBOY_KEY_B);
        }

        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
        {
            press(SELECT);
            xTaskNotifyGive(screenTaskHandle);
        }
        else
        {
            release(SELECT);
        }

        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET)
        {
            press(START);
            xTaskNotifyGive(screenTaskHandle);
        }
        else
        {
            release(START);
        }

        vTaskDelete(NULL);
    }
}


void createJoystickTask(){
	xTaskCreate(controlsJoystickTask, "controlsJoystickTask", 128, NULL, 1, NULL);

}

