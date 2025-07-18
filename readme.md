# ZJU-Dumb-Motor

这个项目包含了浙江大学光电信息科学与工程专业短学期课程《电子系统设计》课程作业，**智障小车的扩展功能开发 (Development of Extended Functions of the Dumb Motor, D.E.F.D.M.)**，相关的代码。

`Keil-Examples`目录下包含了老师提供的全部基本功能演示程序代码，`Basic-Functions`目录下包含了我们重写的基本功能实现，迁移到了较现代的嵌入式开发工具链，`Dumb-Motor-Core`目录包含了我们最终用以测试的程序，去除了实验性和不稳定的功能实现。 我们的小车在测试中展现了使用的障碍物避让以及道路保持功能。

**Release**中提供了可供直接烧录的`.hex`和`.elf`文件。

## 开发环境

- 主控芯片:STM32F103RCT6
- IDE:CLion + STM32CubeMX
- 编译器:Arm GCC
- 调试工具:OpenOCD + ST-Link
