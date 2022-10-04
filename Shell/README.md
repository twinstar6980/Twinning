# TwinKleS ToolKit - Shell

面向用户的外壳程序，提供基于终端的命令行界面。

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

| 库 | 用途 |
|:--:|:----:|

## 说明
