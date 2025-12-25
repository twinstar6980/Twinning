# Assistant

助理，实现了额外的高级辅助功能。

本项目是跨平台的 `Flutter` 项目，依赖 `Kernel` 、`Script` 的实现。

## 项目开发

* 要求
	
	* [Flutter 3.38](https://docs.flutter.dev/get-started/install)

## 第三方库使用

> 参见 [pubspec.yaml](./pubspec.yaml) 。

## 说明

* 内置的动态库依赖
	
	应用内包含了 `Kernel` 动态库所依赖的库文件，这些库文件应与 `Kernel` 编译时所依赖的相匹配。
	
	* `Windows` - `libc++.dll` & `libunwind.dll` @ `llvm-mingw-20251216-ucrt-x86_64` 。
	
	* `Android` - `libc++_shared.so` @ `Android NDK 29.0` 。
