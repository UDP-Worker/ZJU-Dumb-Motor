#include "motor.h"
#include "main.h"
#include <stdlib.h>

static uint16_t pwm_period = 0;

void Motor_Init(void)
{
    MX_TIM4_Init();
    MX_GPIO_Init();
    pwm_period = htim4.Init.Period;
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void Motor_SetSpeed(Motor_Channel ch, int8_t speed)
{
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;

    uint16_t compare = (uint16_t)((pwm_period + 1) * abs(speed) / 100);

    if (ch == MOTOR_RIGHT) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, compare);
        if (speed > 0)
            HAL_GPIO_WritePin(RIGHT_DIR_GPIO_Port, RIGHT_DIR_Pin, GPIO_PIN_RESET);
        else if (speed < 0)
            HAL_GPIO_WritePin(RIGHT_DIR_GPIO_Port, RIGHT_DIR_Pin, GPIO_PIN_SET);
    } else {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, compare);
        if (speed > 0)
            HAL_GPIO_WritePin(LEFT_DIR_GPIO_Port, LEFT_DIR_Pin, GPIO_PIN_SET);
        else if (speed < 0)
            HAL_GPIO_WritePin(LEFT_DIR_GPIO_Port, LEFT_DIR_Pin, GPIO_PIN_RESET);
    }
}

#define FORWARD_TIME_MS 100
#define BACKWARD_TIME_MS 100
#define TURN_TIME_MS 260
#define BRAKE_TIME_MS 100

void Motor_Forward(void)
{
    Motor_SetSpeed(MOTOR_LEFT, -52);
    Motor_SetSpeed(MOTOR_RIGHT, 50);
    HAL_Delay(FORWARD_TIME_MS);
}

void Motor_Backward(void)
{
    Motor_SetSpeed(MOTOR_LEFT, 50);
    Motor_SetSpeed(MOTOR_RIGHT, -52);
    HAL_Delay(BACKWARD_TIME_MS);
}

void Motor_TurnLeft(void)
{
    Motor_SetSpeed(MOTOR_LEFT, -50);
    Motor_SetSpeed(MOTOR_RIGHT, 0);
    HAL_Delay(TURN_TIME_MS);
}

void Motor_TurnRight(void)
{
    Motor_SetSpeed(MOTOR_LEFT, 0);
    Motor_SetSpeed(MOTOR_RIGHT, 50);
    HAL_Delay(TURN_TIME_MS);
}

void Motor_Brake(void)
{
    Motor_SetSpeed(MOTOR_LEFT, 100);
    Motor_SetSpeed(MOTOR_RIGHT, -100);
    HAL_Delay(BRAKE_TIME_MS);
}
