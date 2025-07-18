# STM32CubeMX 配置步骤

以下内容说明如何在 STM32CubeMX 中创建用于 `F06_HYPERSONIC` 工程的 `.ioc` 文件：

1. **新建工程**
   - 打开 STM32CubeMX，选择 **New Project**。
   - 在 MCU/MPU Selector 中选择 `STM32F103RCT6`（LQFP64）。

2. **配置时钟**
   - 在 **Clock Configuration** 中启用外部高速晶振 (HSE 8MHz)。
   - 使能 PLL，倍频系数设置为 9，将系统时钟调整为 72MHz。

3. **配置 GPIO**
   - `PC0` 设为 **GPIO_Output**，用于超声波 TRIG。
   - `PC1` 设为 **GPIO_Input** 并启用外部中断 **EXTI1**，下拉模式，用作超声波 ECHO。
   - `PC3` 设为 **GPIO_Output**，驱动蜂鸣器。

4. **配置定时器**
   - 启用 **TIM2**，模式选择 **Internal Clock**，Prescaler 设置为 `7199`，Period 设置为 `5000`，用于测量回波时间。
   - 在 **NVIC Settings** 中启用 `TIM2` 和 `EXTI1` 中断。

5. **生成代码**
   - 在 **Project Manager** 中设置工程名称 `F06_HYPERSONIC`，生成方式可选择 Makefile。
   - 生成代码后，将生成的 `Core` 和 `Drivers` 目录替换到本工程对应位置。

完成以上步骤后即可在 CLion 中编译和调试该项目。
