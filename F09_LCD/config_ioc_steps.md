# STM32CubeMX 配置步骤

以下步骤演示如何在 STM32CubeMX 中生成 `F09_LCD` 工程的 `.ioc` 文件：

1. **新建工程**
   - 打开 STM32CubeMX，选择 `STM32F103RCT6`（LQFP64 封装）。
2. **配置时钟**
   - 在 **Clock Configuration** 页启用外部高速晶振 (HSE, 8MHz)。
   - 使能 PLL，倍频系数设为 9，使系统时钟达到 72MHz。
3. **配置 GPIO**
   - 将以下引脚设为 `GPIO_Output` 并在 `User Label` 中注明用途：
     - `PA3` : `LCD_RST`
     - `PA4` : `LCD_CLK`
     - `PA5` : `LCD_CS`
     - `PA6` : `LCD_DIO`
     - `PB9` : `LCD_PSB`
   - 若需要按键功能，可将 `PA0`~`PA2` 和 `PB0`~`PB7` 设为 `GPIO_Input`，并启用上拉。
4. **配置串口**
   - 启用 `USART1`，`PA9` 为 TX，`PA10` 为 RX，波特率 115200。
5. **生成代码**
   - 在 **Project Manager** 中设置工程名称 `F09_LCD`，选择生成 `Makefile` 工程。
   - 点击 **Generate Code**，生成的 `Core` 与 `Drivers` 覆盖到本仓库对应位置即可。
