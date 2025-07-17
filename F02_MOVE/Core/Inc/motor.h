#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"
#include "tim.h"
#include "gpio.h"

typedef enum {
    MOTOR_RIGHT = 0,
    MOTOR_LEFT
} Motor_Channel;

void Motor_Init(void);
void Motor_SetSpeed(Motor_Channel ch, int8_t speed);

void Motor_Forward(void);
void Motor_Backward(void);
void Motor_TurnLeft(void);
void Motor_TurnRight(void);
void Motor_Brake(void);

#endif
