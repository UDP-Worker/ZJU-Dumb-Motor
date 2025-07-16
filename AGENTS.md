# ZJU-Dumb-Motor

这个仓库包含一些库函数编写的keil工程，我希望将其逐步重构成现代化的嵌入式工程，我的开发环境是:

```
IDE:CLion + STM32CubeMX
编译器:Arm GCC
调试工具:OpenOCD + ST-Link
```

在你写代码之前，总是运行`install_cmake3.31.sh`以确保你有正确版本的cmake可以使用。

另外，请总是确保工程子目录下/Core/Src/stm32f1xx_hal_msp.c下的第76行`__HAL_AFIO_REMAP_SWJ_DISABLE();`被注释掉，以防止后续无法烧录程序

为此，在你执行任务的过程中，我希望你以现代化的方式重构示例代码，考虑到你无法直接使用STM32CUBEMX，你不应该编写初始化代码等应当由STM32CUBEMX编写的代码，相反，你应该在每个工程子目录下，新建一个`config_ioc_steps.md`，以简体中文向用户解释如何配置`.ioc`，然后由用户通过STM32CUBEMX来生成代码。

如果你在执行重构代码时发现需要一些由STM32CUBEMX生成的代码，不要编写它们，相反，你应当在工程子目录下，新建一个`config_ioc_request.md`，以简体中文向用户描述应如何额外配置`.ioc`，然后由用户通过STM32CUBEMX来生成代码。