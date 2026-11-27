# Kernel

内核，负责内部功能的实现。

本项目是跨平台的 `C++` 项目，不依赖其他模块的实现。

## 项目开发

* 要求

	* [xmake 3.1](https://xmake.io/#/)

	* [LLVM 23.1 (LLVM-MinGW-ucrt-20260908)](https://github.com/mstorsjo/llvm-mingw) for `Windows`

	* [LLVM 23.1](https://llvm.org/) for `Linux`

	* [LLVM 23.1 (SDK 26.5)](https://llvm.org/) for `Macintosh`

	* [LLVM 21.0 (NDK 30.0)](https://developer.android.com/ndk/downloads) for `Android`

	* [LLVM 23.1 (SDK 26.5)](https://llvm.org/) for `Iphone`

## 第三方库使用

参见 [dependency](./dependency) 。

## 说明

* 关于接口

	* 本项目被编译为动态库，需要调用方进行动态加载。

	* 接口声明位于 `<project>/kernel/interface/interface.hpp` 。

	* 为了允许来自不同语言的调用，接口被设计为完全的 `C` 风格。输入参数的所有权归调用方，调用方需确保其能够正确析构；输出参数与返回值的所有权属于库自身，调用方需通过 `Executor::clear` 函数对其进行析构。

	* 接口具备线程安全性，但不建议在多个线程中同时调用同一库实例的接口，因为内部使用的脚本引擎 `quickjs_ng` 只能被不同线程互斥地调用；如需在多线程中调用接口，请为每个线程加载独立的库实例。

* 关于构建

	* 本项目无法通过 GCC 与高版本 MSVC（17.11.5|19.41.34123+）的编译，这是由于 GCC 与 MSVC 在处理几处较为复杂的代码（特别是模板与模块）时出现了 BUG 。
