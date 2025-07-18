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

    uint16_t compare = (uint16_t)((pwm_period + 1) * (100 - abs(speed)) / 100);

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

void Motor_Run(int8_t speed, uint32_t time_ms)
{
    Motor_SetSpeed(MOTOR_LEFT, -speed);
    Motor_SetSpeed(MOTOR_RIGHT, speed);
    HAL_Delay(time_ms);
}

void Motor_Brake(uint32_t time_ms)
{
    Motor_SetSpeed(MOTOR_LEFT, 0);
    Motor_SetSpeed(MOTOR_RIGHT, 0);
    HAL_Delay(time_ms);
}

void Motor_Left(int8_t speed, uint32_t time_ms)
{
    Motor_SetSpeed(MOTOR_LEFT, 0);
    Motor_SetSpeed(MOTOR_RIGHT, speed);
    HAL_Delay(time_ms);
}

void Motor_SpinLeft(int8_t speed, uint32_t time_ms)
{
    Motor_SetSpeed(MOTOR_LEFT, speed);
    Motor_SetSpeed(MOTOR_RIGHT, speed);
    HAL_Delay(time_ms);
}

void Motor_Right(int8_t speed, uint32_t time_ms)
{
    Motor_SetSpeed(MOTOR_LEFT, -speed);
    Motor_SetSpeed(MOTOR_RIGHT, 0);
    HAL_Delay(time_ms);
}

void Motor_SpinRight(int8_t speed, uint32_t time_ms)
{
    Motor_SetSpeed(MOTOR_LEFT, -speed);
    Motor_SetSpeed(MOTOR_RIGHT, -speed);
    HAL_Delay(time_ms);
}

void Motor_Back(int8_t speed, uint32_t time_ms)
{
    Motor_SetSpeed(MOTOR_LEFT, speed);
    Motor_SetSpeed(MOTOR_RIGHT, -speed);
    HAL_Delay(time_ms);
}
