# STM32CubeMX 配置步骤

下列步骤演示如何在 STM32CubeMX 中生成 `07_SERVO` 重构工程所需的 `.ioc` 文件，适用于 STM32F103RCT6 开发板。

1. **新建工程**
   - 打开 STM32CubeMX，点击 **New Project**。
   - 在 MCU/MPU Selector 中搜索 `STM32F103RCT6` 并选择该型号。

2. **配置时钟**
   - 在 **Clock Configuration** 页面将系统时钟设为 72 MHz。
   - 采用外部 8 MHz 晶振并启用 PLL，倍频系数为 9。

3. **配置 TIM4 PWM**
   - 启用 **TIM4** 的 **Channel3** 和 **Channel4**，模式选择 **PWM Generation CH3/CH4**。
   - 通道分别连接 `PB8` 与 `PB9`，将 **Prescaler** 设为 `0`，**Counter Period** 设为 `7199`，获得约 10 kHz PWM。

4. **配置电机方向控制引脚**
   - `PB7`、`PA4` 设为 **GPIO_Output**，用于电机方向控制。

5. **配置舵机控制**
   - 启用 **TIM5** 的 **Channel1**，模式选择 **PWM Generation CH1**，引脚使用 `PA0`。
   - 将 **Prescaler** 设为 `143`，**Counter Period** 设为 `9999`，得到约 50 Hz 的 PWM 输出。

6. **配置超声波测距模块与定时器**
   - `PC0` 设为 **GPIO_Output** (TRIG)。
   - `PC1` 设为 **GPIO_Input**，启用 **EXTI Line1**，触发方式 Rising Edge。
   - 启用 **TIM2**，`Prescaler` 设为 `7199`，`Counter Period` 设为 `5000`，并使能中断以测量 ECHO 脉宽。

7. **配置串口**
   - **USART1**：`PA9` (TX) 与 `PA10` (RX)，波特率 115200。

8. **配置红外传感器和其他外设**
   - `PA7`、`PB0` 作为循迹传感器输入，`PB1` 为避障传感器输入，均上拉。
   - 可选 `PC2` 作为按键，`PC3` 作为蜂鸣器输出。

9. **生成代码**
   - 在 **Project Manager** 中设置工程名称以及生成选项，最后点击 **Generate Code**。

