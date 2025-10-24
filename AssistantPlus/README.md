# Assistant Plus

助理+，面向 Windows 平台的特化版本。

本项目是面向 `Windows` 的 `C++/C#` 项目，依赖 `Kernel` 、`Script` 的实现。

## 项目开发

* 要求
	
	* [Visual Studio 2022 17.14](https://visualstudio.microsoft.com/downloads/)

## 第三方库使用

> 参见 [Application.csproj](./Application/Application.csproj) 。

## 说明

* 内置的动态库依赖
	
	应用内包含了 `Kernel` 动态库所依赖的库文件，这些库文件应与 `Kernel` 编译时所依赖的相匹配。
	
	* `libc++.dll` & `libunwind.dll` @ `llvm-mingw-20251021-ucrt-x86_64` 。
