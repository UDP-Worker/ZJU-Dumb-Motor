#include "uart_io.h"
#include <string.h>

static UART_HandleTypeDef *debug_huart = NULL;

void UART_IO_Init(UART_HandleTypeDef *huart)
{
    debug_huart = huart;
}

int __io_putchar(int ch)
{
    uint8_t c = (uint8_t)ch;
    if (debug_huart)
    {
        HAL_UART_Transmit(debug_huart, &c, 1, HAL_MAX_DELAY);
    }
    return ch;
}
