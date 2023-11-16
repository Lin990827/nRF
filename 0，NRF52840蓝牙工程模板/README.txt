说明：NRF52840 BLE工程模板

工程文件目录
	components：该文件包存放的是各类驱动、蓝牙协议栈、芯片库程序等文件，是后期我们编程所需要的文件，是SDK的核心部分。
	drive：所编写的一些设备驱动的存放文件夹。
	examples：包含user文件夹和main.c和main.h文件。user文件夹下有两个文件夹config和project，config用于存放提供开发环境
			以及库函数的配置（芯片配置使能模板），project用于存放keil工程文件。
	external：第三方的驱动文件。
	integration：老版本驱动的兼容头文件。
	modules：各种环境中使用的驱动程序。
	.clang-format：代码格式化配置文件。
	keilkilll.bat：清除编译后产生文件的脚本。