# STM32CubeMX 配置步骤

以下步骤演示如何在 STM32CubeMX 中生成 `F08_USART` 工程的 `.ioc` 文件，用于 STM32F103RCT6 开发板：

1. **新建工程**
   - 打开 STM32CubeMX，点击 **New Project**。
   - 在 MCU/MPU Selector 中选择 `STM32F103RCT6`（LQFP64）。

2. **配置时钟**
   - 在 **Clock Configuration** 页面启用外部高速晶振 (HSE, 8MHz)。
   - 启用 PLL，倍频系数设置为 9，使系统时钟达到 72MHz。

3. **配置 GPIO 与定时器**
   - `PC0` 设为 **GPIO_Output**，用作超声波 TRIG。
   - `PC1` 设为 **GPIO_Input** 并启用 **EXTI Line1**，用作超声波 ECHO。
   - 启用 **TIM2**，`Prescaler` 设为 `7199`，`Counter Period` 设为 `4999`，用于测量回波时间。
   - 在 **NVIC Settings** 中启用 `TIM2` 与 `EXTI1` 中断。

4. **配置串口**
   - 启用 **USART1**，引脚 `PA9` 作为 TX、`PA10` 作为 RX，波特率设置为 115200。

5. **生成代码**
   - 在 **Project Manager** 中设置工程名称 `F08_USART`，生成方式可以选择 Makefile。
   - 点击 **Generate Code**，将生成的 `Core` 和 `Drivers` 目录替换到本工程相应位置。

完成以上步骤后即可在 CLion 中编译和调试该项目。
