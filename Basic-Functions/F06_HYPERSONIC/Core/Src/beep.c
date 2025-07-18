#include "beep.h"

void Beep_Init(void)
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
}

void Beep_On(void)
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
}

void Beep_Off(void)
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
}

void Beep_Toggle(void)
{
    HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);
}
