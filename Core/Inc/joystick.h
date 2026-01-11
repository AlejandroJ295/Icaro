// SPDX-License-Identifier: GPL-3.0-only

#ifndef CBOY_JOYSTICK_H
#define CBOY_JOYSTICK_H

#include "FreeRTOS.h"

void init_joystick();
void controlsJoystickTask(void * pargs);
void createJoystickTask();

#endif // CBOY_JOYSTICK_H
