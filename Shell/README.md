# TwinKleS ToolKit - Shell

命令行 UI。

此为跨平台 C++ 项目。

## 项目构建

* 构建系统

	CMake 3.21+ ，可使用 VS / CLion 等IDE进行开发调试。

* 编译器要求

	MSVC 19.33+ 或 Clang 15+ ，不支持 GCC 。

* 构建脚本

	项目中提供了PowerShell脚本以快速构建项目，可用于 Windows / Linux / MacOS 的本地编译与面向于 Android 的交叉编译。
	
	命令格式： ./build_script/build_preset.ps1 -system [native | android] -build [debug | release]

	执行命令后，构建输出至目录 ./build 中。
