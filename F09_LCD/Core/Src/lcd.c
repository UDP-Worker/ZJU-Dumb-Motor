#include "lcd.h"

static uint8_t current_row = 0;
static uint8_t current_col = 0;

static const uint8_t row_addr[4] = {0x80, 0x90, 0x88, 0x98};

static void lcd_send_byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; ++i)
    {
        HAL_GPIO_WritePin(LCD_DIO_GPIO_Port, LCD_DIO_Pin,
                          (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_SET);
        data <<= 1;
    }
}

static void lcd_write_command(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
    lcd_send_byte(0xF8);
    lcd_send_byte(cmd & 0xF0);
    lcd_send_byte((cmd << 4) & 0xF0);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(2);
}

static void lcd_write_data(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
    lcd_send_byte(0xFA);
    lcd_send_byte(data & 0xF0);
    lcd_send_byte((data << 4) & 0xF0);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(2);
}

static void lcd_set_address(uint8_t row, uint8_t col)
{
    lcd_write_command(0x30);
    lcd_write_command(row_addr[row] + col);
}

void LCD_Init(void)
{
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(90);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(90);

    lcd_write_command(0x30);
    HAL_Delay(2);
    lcd_write_command(0x06);
    HAL_Delay(2);
    lcd_write_command(0x01);
    HAL_Delay(2);
    lcd_write_command(0x0C);
    HAL_Delay(2);
    lcd_write_command(0x02);
    HAL_Delay(2);
}

void LCD_Clear(void)
{
    lcd_write_command(0x01);
    HAL_Delay(2);
    current_row = 0;
    current_col = 0;
}

static void lcd_put_char(char ch)
{
    if (current_col >= 16)
    {
        current_col = 0;
        current_row = (current_row + 1) % 4;
    }
    lcd_set_address(current_row, current_col);
    lcd_write_data((uint8_t)ch);
    current_col++;
}

void LCD_Print(const char *str)
{
    while (*str)
    {
        if (*str == '\n')
        {
            current_row = (current_row + 1) % 4;
            current_col = 0;
            lcd_set_address(current_row, current_col);
        }
        else
        {
            lcd_put_char(*str);
        }
        str++;
    }
}
