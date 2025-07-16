# STM32CubeMX 配置步骤

以下说明如何在 STM32CubeMX 中创建用于重构 `06_HYPERSONIC` 工程的 `.ioc` 文件。请根据实际硬件做适当调整。

1. **新建工程**
   - 打开 STM32CubeMX，选择 **New Project**。
   - 在 MCU/MPU Selector 中搜索并选择 `STM32F103RCT6`（LQFP64 封装）。

2. **配置时钟**
   - 在 **Clock Configuration** 标签页中，将系统时钟配置为 72 MHz。
   - 使用外部高速晶振 (HSE 8 MHz) 并启用 PLL，PLL 倍频系数设为 9。

3. **配置 TIM4 PWM**
   - 在 **Pinout & Configuration** 中启用 **TIM4**，将 **Channel3** 和 **Channel4** 设置为 **PWM Generation CH3/CH4**。
   - 这两个通道连接 `PB8`、`PB9`，在参数中将 **Prescaler** 设为 `0`，**Counter Period** 设为 `7199`，输出约 10 kHz 的 PWM。

4. **配置电机方向控制引脚**
   - `PB7` 与 `PA4` 设置为 **GPIO_Output**，推挽方式。

5. **配置红外传感器输入**
   - `PA7`、`PB0` 用于循迹或搜索，`PB1` 用于避障，均配置为上拉输入。

6. **配置超声波测距模块**
   - `PC0` 设为 **GPIO_Output** 用作 TRIG。
   - `PC1` 设为 **GPIO_Input**，并启用外部中断 **EXTI Line1**，触发方式选择 Rising Edge。
   - 启用 **TIM2**，在参数中将 **Prescaler** 设为 `7199`，**Counter Period** 设为 `5000`，并使能更新中断。

7. **配置串口**
   - 启用 **USART1**，`PA9` 为 TX，`PA10` 为 RX，波特率设为 115200。

8. **按键和蜂鸣器（可选）**
   - `PC2` 作为按键输入（上拉或带中断）。
   - `PC3` 作为蜂鸣器输出。

9. **生成代码**
   - 在 **Project Manager** 设置工程名称和输出方式，然后点击 **Generate Code** 生成 HAL 初始化代码。

