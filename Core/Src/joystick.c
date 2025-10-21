/*
 * joystick.c
 *
 *  Created on: Oct 15, 2025
 *      Author: zomeg
 */

void controlsJoystickTask(void * pargs){

	while(1){
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

		    vTaskDelete(NULL);
	}
}

void createJoystickTask(){
	xTaskCreate(controlsJoystickTask, "controlsJoystickTask", 128, NULL, 1, NULL);

}

