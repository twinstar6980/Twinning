# TwinKleS ToolKit - Core

核心功能的实现。

此为跨平台 C++ 项目。

## 项目构建

* 构建系统

	CMake 3.21+ ，可使用 VS / CLion 等IDE进行开发调试。

* 编译器要求

	MSVC 19.33+ 或 Clang 16+(dev) ，不支持 GCC 。

* 构建脚本

	项目中提供了PowerShell脚本以快速构建项目，可用于 Windows / Linux / MacOS 的本地编译与面向于 Android 的交叉编译。
	
	命令格式： ./build_script/build_preset.ps1 -system [native | android] -build [debug | release]

	执行命令后，构建输出至目录 ./build 中。

* 调试注意
	
	项目使用VS+VLD进行开发与内存泄漏检测，由于VLD无法检测动态库的内存泄露情况，因此在调试时需将/core子项目编译为静态库使用。

## 第三方库使用

| 库                                                             | 用途              |
|:--------------------------------------------------------------:|:-----------------:|
| [fmt 9.0.0](https://github.com/fmtlib/fmt)                     | 字符串格式化        |
| [tinyxml2 9.0.0](https://github.com/leethomason/tinyxml2)      | XML读写           |
| [quickjs](https://github.com/bellard/quickjs)                  | JS脚本引擎         |
| [quickjs (c-smile)](https://github.com/c-smile/quickjspp)      | JS脚本引擎(MSVC)   |
| [md5](https://github.com/JieweiWei/md5)                        | MD5哈希计算        |
| [Rijndael](_)                                                  | Rijndael加密与解密 |
| [zlib 1.2.12](https://www.zlib.net/)                           | ZLib压缩与解压     |
| [bzip2 1.0.8](https://sourceware.org/bzip2/)                   | BZip2压缩与解压    |
| [lzma 22.01](https://www.7-zip.org/sdk.html)                   | Lzma压缩与解压     |
| [libpng 1.6.37](http://www.libpng.org/pub/png/libpng.html)     | PNG读写           |
| [ETCPACK 2.74](https://github.com/Ericsson/ETCPACK)            | ETC1解压          |
| [rg_ect1](https://github.com/richgel999/rg-etc1)               | ETC1压缩          |
| [PVRTCCompressor](https://github.com/brenwill/PVRTCCompressor) | PVRTC4压缩与解压   |
| [vld](https://github.com/KindDragon/vld)                       | 内存泄漏检测       |

## API说明

1. API 头文件位于 /core/interface/interface.hpp 。

2. 由于需要容许从 C/C# 处的调用，故 API 被设计为完全的 C 风格。参数变量的所有权归调用方，调用方需确保其能够正确析构；而返回值变量的所有权归 API 自身，调用方不应对其进行读取以外的任何操作。

3. /core 子项目被编译为动态库，需要调用方进行动态加载。
