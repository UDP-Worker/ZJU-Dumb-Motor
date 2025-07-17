# STM32CubeMX 配置步骤

下列步骤说明如何在 STM32CubeMX 中为 `Dumb-Motor` 综合小车工程创建 `.ioc` 文件。硬件连接与之前的 `F02_MOVE`、`F04_FOLLOWLINE`、`F06_HYPERSONIC`、`F07_SERVO` 完全一致。

1. **新建工程**
   - 打开 STM32CubeMX，选择 **New Project**。
   - 在 MCU/MPU Selector 中搜索并选中 `STM32F103RCT6`（LQFP64 封装）。

2. **配置时钟**
   - 在 **Clock Configuration** 标签页启用外部高速晶振 (HSE 8 MHz)。
   - 启用 PLL，倍频系数设为 9，使系统时钟达到 72 MHz。

3. **配置电机驱动 PWM (TIM4)**
   - 在 **Pinout & Configuration** 中启用 **TIM4** 的 **Channel3** 与 **Channel4**，模式选择 **PWM Generation CH3/CH4**。
   - 将两路分别映射到 `PB8`、`PB9`，在 **Parameter Settings** 中将 **Prescaler** 设为 `0`，**Counter Period** 设为 `7199`，得到约 10 kHz 的 PWM。
   - `PB7` 与 `PA4` 配置为 **GPIO_Output**，用于电机方向控制。

4. **配置循迹红外传感器**
   - 将 `PA7` 与 `PB0` 设置为 **GPIO_Input**，模式选择上拉，用于左、右循迹传感器。
   - 可选 `PC2` 配置为按键输入（上拉），`PC3` 配置为蜂鸣器输出。

5. **配置舵机控制 (TIM5)**
   - 启用 **TIM5** 的 **Channel1**，模式选择 **PWM Generation CH1**，引脚使用 `PA0`。
   - 将 **Prescaler** 设为 `143`，**Counter Period** 设为 `9999`，产生约 50 Hz 的 PWM 驱动舵机。

6. **配置超声波测距模块**
   - `PC0` 设置为 **GPIO_Output**，用作 TRIG 信号。
   - `PC1` 设置为 **GPIO_Input** 并启用 **EXTI Line1**（上升沿触发）作为 ECHO 输入。
   - 启用 **TIM2**，在 **Parameter Settings** 中将 **Prescaler** 设为 `7199`，**Counter Period** 设为 `5000`，并在 **NVIC Settings** 中使能 `TIM2` 和 `EXTI1` 中断，用于测量回波时间。

7. **配置串口调试 (USART1)**
   - 启用 **USART1**，`PA9` 作为 TX，`PA10` 作为 RX，波特率设为 115200。

8. **检查中断和时钟**
   - 保持 `SysTick` 中断开启以提供 1 ms 系统节拍。
   - CubeMX 会自动启用 GPIO、定时器和 USART 的时钟，如有需要可在 **RCC** 与 **NVIC Settings** 中确认。

9. **生成代码**
   - 在 **Project Manager** 中设置工程名称（例如 `DumbMotor`），选择生成方式（如 Makefile 或 STM32CubeIDE）。
   - 点击 **Generate Code**。生成的 `Core` 与 `Drivers` 目录即可用于后续在 CLion 中开发。

