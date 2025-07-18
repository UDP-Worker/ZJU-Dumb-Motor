#include "motor.h"
#include "main.h"
#include <stdlib.h>

extern TIM_HandleTypeDef htim4;
static uint16_t pwm_period = 0;

void Motor_Init(void)
{
    pwm_period = htim4.Init.Period;
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

static void motor_set(Motor_Channel ch, int8_t speed)
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

void motor_fwd(void) {
    motor_set(MOTOR_LEFT, -50);
    motor_set(MOTOR_RIGHT, 50);
}

void motor_bwd(void) {
    motor_set(MOTOR_LEFT, 50);
    motor_set(MOTOR_RIGHT, -50);
}

void motor_turn_left(void) {
    motor_set(MOTOR_LEFT, -50);
    motor_set(MOTOR_RIGHT, 0);
}

void motor_turn_right(void) {
    motor_set(MOTOR_LEFT, 0);
    motor_set(MOTOR_RIGHT, 50);
}

void motor_brake(void) {
    motor_set(MOTOR_LEFT, 0);
    motor_set(MOTOR_RIGHT, 0);
}
