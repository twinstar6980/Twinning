# Assistant Plus

助理+，面向 Windows 平台的特化版本。

本项目是面向 `Windows` 的 `WinUI 3` 项目，依赖 `Kernel` 、`Script` 的实现。

## 项目开发

* 要求

	* [Visual Studio 2026 18.3](https://visualstudio.microsoft.com/downloads/)

## 第三方库使用

> 参见 [Directory.Packages.props](./Directory.Packages.props) 。

## 说明

* 内置的动态库依赖

  应用构建时会自动将当前编译环境中的 `C++` 共享库打包进安装包内，这些库是动态加载 `Kernel` 所必需的依赖，应用只应加载相同依赖下编译出的 `Kernel` 。
