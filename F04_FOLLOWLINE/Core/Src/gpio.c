#include "gpio.h"

void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(RIGHT_DIR_GPIO_Port, RIGHT_DIR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LEFT_DIR_GPIO_Port, LEFT_DIR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = RIGHT_DIR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RIGHT_DIR_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LEFT_DIR_Pin;
    HAL_GPIO_Init(LEFT_DIR_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = BEEP_Pin;
    HAL_GPIO_Init(BEEP_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KEY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IR_RIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IR_RIGHT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = IR_LEFT_Pin;
    HAL_GPIO_Init(IR_LEFT_GPIO_Port, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(KEY_EXTI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(KEY_EXTI_IRQn);
}
