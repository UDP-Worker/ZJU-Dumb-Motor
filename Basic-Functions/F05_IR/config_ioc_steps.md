# STM32CubeMX 配置步骤

以下步骤说明如何在 STM32CubeMX 中创建用于重构 `05_IR` 工程的 `.ioc` 文件，请根据实际使用的开发板做适当调整。

1. **新建工程**
   - 打开 STM32CubeMX，选择 **New Project**。
   - 在 MCU/MPU Selector 中搜索并选择 `STM32F103RCT6`（LQFP64 封装）。

2. **配置时钟**
   - 在 **Clock Configuration** 标签页中，将系统时钟配置为 72 MHz。
   - 使用外部高速晶振 (HSE 8 MHz) 并启用 PLL，PLL 倍频系数设为 9。

3. **配置 TIM4 PWM**
   - 在 **Pinout & Configuration** 中启用 **TIM4**，将 **Channel3** 和 **Channel4** 设置为 **PWM Generation CH3/CH4**。
   - 这两个通道分别映射到 `PB8` 和 `PB9`。
   - 在 **Parameter Settings** 中，将 TIM4 的 **Prescaler** 设为 `0`，**Counter Period** 设为 `7199`，生成约 10 kHz 的 PWM。

4. **配置电机方向控制引脚**
   - 将 `PB7` 和 `PA4` 设置为 **GPIO_Output**，推挽输出，速度可设为 Low 或 High。

5. **配置红外传感器输入**
   - 将 `PA7` 和 `PB0` 用作循迹/搜索红外传感器输入，模式选择 **GPIO_Input**，并启用上拉电阻。
   - 将 `PB1` 用作避障红外传感器输入，模式同上。

6. **按键和蜂鸣器（可选）**
   - 如需调试，可将 `PC2` 配置为 **GPIO_Input** 上拉模式，用作按键。
   - `PC3` 可配置为 **GPIO_Output**，用于驱动蜂鸣器。

7. **生成代码**
   - 在 **Project Manager** 中设置工程名称和生成方式（如 Makefile）。
   - 点击 **Generate Code** 生成初始化代码，随后即可在 CLion 等环境中继续开发。

