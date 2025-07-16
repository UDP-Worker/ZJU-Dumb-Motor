# STM32CubeMX 配置步骤

以下步骤说明如何在 STM32CubeMX 中创建用于重构 `04_FOLLOWLINE` 工程的 `.ioc` 文件。请根据实际的开发板型号进行微调。

1. **新建工程**
   - 打开 STM32CubeMX，选择 **New Project**。
   - 在 MCU/MPU Selector 中搜索并选择 `STM32F103RCT6`（LQFP64 封装）。

2. **配置时钟**
   - 在 **Clock Configuration** 标签页中，将系统时钟配置为 72 MHz。
   - 使用外部高速晶振 (HSE 8 MHz) 并启用 PLL，PLL 倍频系数为 9。

3. **配置 TIM4 PWM**
   - 在 **Pinout & Configuration** 中启用 **TIM4**，将 **Channel3** 和 **Channel4** 设为 **PWM Generation CH3/CH4**。
   - 分别映射到 `PB8` 和 `PB9`。
   - 在 **Parameter Settings** 中，将 TIM4 的 **Prescaler** 设为 `0`，**Counter Period** 设为 `7199`，得到约 10 kHz 的 PWM 频率。

4. **配置电机方向控制引脚**
   - 将 `PB7` 和 `PA4` 设置为 **GPIO_Output**，输出模式选择 **Push-Pull**，速度设置为 **Low** 或 **High** 均可。

5. **配置循迹红外传感器**
   - 将 `PA7` 和 `PB0` 设置为 **GPIO_Input**，启用上拉电阻，用于读取左右循迹红外传感器的状态。

6. **配置蜂鸣器与按键**
   - 将 `PC3` 设置为 **GPIO_Output**，用于驱动蜂鸣器。
   - 将 `PC2` 设置为 **GPIO_Input**，上拉模式，可根据需要启用 EXTI2 中断实现按键检测。

7. **开启必要的中断与外设时钟**
   - 在 **NVIC Settings** 中保持 `SysTick` 中断开启。如使用 EXTI2，可在此启用对应中断。
   - CubeMX 会自动启用 GPIOA、GPIOB、GPIOC 以及 TIM4 的时钟，无需额外设置。

8. **生成代码**
   - 在 **Project Manager** 中，设定工程名称（例如 `DumbMotor`），选择生成工具链（如 `STM32CubeIDE` 或 `Makefile`）。
   - 点击 **Generate Code**，生成的工程即包含完成的初始化代码，可在 CLion 中继续开发。

完成以上配置后，即可使用生成的代码替换旧的 Keil 工程，实现 `04_FOLLOWLINE` 的现代化重构。
