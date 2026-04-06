# Assistant

助理，实现了额外的高级辅助功能。

本项目是跨平台的 `Flutter` 项目，依赖 `Kernel` 、`Script` 的实现。

## 项目开发

* 要求

	* [Flutter 3.41](https://docs.flutter.dev/get-started/install)

## 第三方库使用

> 参见 [pubspec.yaml](./pubspec.yaml) 。

## 说明

* 内置的动态库依赖

  应用构建时会自动将当前编译环境中的 `C++` 共享库打包进安装包内，这些库是动态加载 `Kernel` 所必需的依赖，应用只应加载相同依赖下编译出的 `Kernel` 。

* 关于构建

	* 为了减小应用包体积，构建面向 `Macintosh` 的程序时不会生成 `amd64` 架构的可执行文件，这一设置被硬编码在项目的 [Release.xcconfig](../Assistant/macos/Runner/Configs/Release.xcconfig) 中，若需要为 `amd64` 构建应用包，请修改该文件。
