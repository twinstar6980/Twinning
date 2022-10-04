# TwinKleS ToolKit - Core

核心功能的实现，以动态库形式分发。

此为跨平台 C++ 项目。

## 项目构建

* 构建系统
	
	CMake 3.21+ ，可使用 VS / CLion 等 IDE 进行开发调试。

* 编译器要求
	
	MSVC 19.33+ 或 Clang 14+ ，不支持 GCC 。

* 构建脚本
	
	项目中提供了 PowerShell 脚本用以快速构建项目。
	
	命令行：
	
	* ./build.ps1 \<project> -system **native** -build \<build> -native_compiler_c \<native_compiler_c> -native_compiler_cxx \<native_compiler_cxx>
		
		用于 Windows / Linux / MacOS 的本地编译
	
	* ./build.ps1 \<project> -system **android** -build \<build> -android_ndk \<android_ndk> -android_api \<android_api> -android_arch \<android_arch>
		
		面向 Android 的交叉编译
	
	* ./build.ps1 \<project> -system **ios** -build \<build> -ios_toolchain \<ios_toolchain> -ios_compiler_c \<ios_compiler_c> -ios_compiler_cxx \<ios_compiler_cxx> -ios_api \<ios_api> -ios_arch \<ios_arch>
		
		面向 iOS 的交叉编译
	
	参数说明：
	
	* \<project>
		
		项目路径。
	
	* \<build>
		
		构建类型，可选值有：debug | release | release_minimum | release_debug 。
	
	* \<*_compiler_c>
		
		C 编译器。
		
		在 Windows 上，必须为 MSVC cl ；在其他系统上，必须为 Clang clang 。
	
	* \<*_compiler_cxx>
		
		C++ 编译器。
		
		在 Windows 上，必须为 MSVC cl ；在其他系统上，必须为 Clang clang++ 。
	
	* \<android_ndk>
		
		用于交叉编译 Android 的 NDK 目录路径。
		
		下载：[NDK](https://developer.android.google.cn/ndk/downloads?hl=zh-cn) 。
	
	* \<ios_toolchain>
		
		用于交叉编译 iOS 的 CMake 工具链文件路径。
		
		下载：[ios-cmake](https://github.com/leetal/ios-cmake) 。
	
	* \<*_api>
		
		面向的最低系统版本。对应于 Android \<X = 32, ...> 与 iOS \<X.Y = 15.0, ...> 。
	
	* \<*_arch>
		
		目标处理器架构，可选值有：x86_32 | x86_64 | arm_32 | arm_64 。
		
		面向 iOS 进行交叉编译时，只可选择 arm_64 架构；本地编译时，目标架构等同于操作系统自身架构。
	
	执行命令后，构建将输出至目录 ./.build 中。

* 调试注意
	
	项目使用 VS + VLD 进行开发与内存泄漏检测，在 Windows 上编译 debug 版本时需确保 [VLD](https://github.com/KindDragon/vld/releases) 已安装至计算机 "C:/Program Files (x86)/Visual Leak Detector" 目录中。

## 第三方库使用

| 库                                                             | 用途                          |
|:--------------------------------------------------------------:|:-----------------------------:|
| [fmt 9.0.0](https://github.com/fmtlib/fmt)                     | 字符串格式化                  |
| [tinyxml2 9.0.0](https://github.com/leethomason/tinyxml2)      | XML读写                       |
| [quickjs](https://github.com/bellard/quickjs)                  | JS脚本引擎                    |
| [quickjs (c-smile)](https://github.com/c-smile/quickjspp)      | JS脚本引擎 ( for MSVC )       |
| [md5](https://github.com/JieweiWei/md5)                        | MD5哈希计算                   |
| [Rijndael](#)                                                  | Rijndael加密与解密            |
| [zlib 1.2.12](https://www.zlib.net/)                           | ZLib压缩与解压                |
| [bzip2 1.0.8](https://sourceware.org/bzip2/)                   | BZip2压缩与解压               |
| [lzma 22.01](https://www.7-zip.org/sdk.html)                   | Lzma压缩与解压                |
| [libpng 1.6.37](http://www.libpng.org/pub/png/libpng.html)     | PNG读写                       |
| [ETCPACK 2.74](https://github.com/Ericsson/ETCPACK)            | ETC1解压                      |
| [rg_ect1](https://github.com/richgel999/rg-etc1)               | ETC1压缩                      |
| [PVRTCCompressor](https://github.com/brenwill/PVRTCCompressor) | PVRTC4压缩与解压              |
| [mscharconv 1.2.1](https://github.com/iboB/mscharconv)         | charconv支持 ( for non-MSVC ) |
| [vld 2.5.1](https://github.com/KindDragon/vld)                 | 内存泄漏检测                  |

## 说明

* 关于 API
	
	1. API 头文件位于 /core/interface/interface.hpp 。
	
	2. 由于需要容许从 C/C# 处的调用，故 API 被设计为完全的 C 风格。参数变量的所有权归调用方，调用方需确保其能够正确析构；而返回值变量的所有权归 API 自身，调用方不应对其进行读取以外的任何操作。
	
	3. /core 子项目被编译为动态库，需要调用方进行动态加载。
