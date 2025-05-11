# Assistant

助理，实现了额外的高级辅助功能。

本项目是跨平台的 `Flutter` 项目，依赖 `Kernel` 、`Script` 的实现。

## 项目开发

* 要求
	
	* [Flutter 3.29](https://docs.flutter.dev/get-started/install)

## 第三方库使用

> 参见 [pubspec.yaml](./pubspec.yaml) 。

## 说明

* 关于 Android 平台的必要说明
	
	自 `Android 7+` 起，系统会对应用的动态库加载行为加以限制，应用只能加载 `default_library_paths` 与 `permitted_paths` 中的库文件，并且对于后者必须指定绝对路径而不允许只指定库名。
	
	因此，为了实现动态加载 Kernel 库文件的需求，应用会在每次运行时将指定的库文件复制至 `/data/user/<user>/<package>/cache` 中（该目录位于 `permitted_paths` 之中）；并且在应用打包时也必须包含一份 `libc++_shared.so` 文件。
	
	> 当前项目中包含的 libc++_shared.so 版本为 NDK 28.1 。
	
	> 具体参阅 [Android 文档](https://source.android.com/docs/core/architecture/vndk/linker-namespace) 。
