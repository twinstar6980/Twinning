# TwinKleS ToolKit - Windows Explorer Extension

适用于 Windows 的资源管理器拓展，将 ToolKit 集成至系统资源管理器的右键菜单中。

此为面向 Windows 的 C++ 项目。

## 项目构建

本项目为 VS 2022 项目。

需要注意的是：项目中的 ./package/Assets/Library/implement.dll 实际上是一个指向 /.build/x64-Release/implement/implement.dll 的符号链接，在首次构建时，应运行 /package/Assets/Library/link.bat 文件以创建这一符号链接，并在每次更新 implement 子项目后重新构建（而非直接构建） package 子项目，从而使 VS 可将新的 implement.dll 打包进 msix 。

## 第三方库使用

| 库 | 用途 |
|:--:|:----:|

## 说明

* 要求 Windows 11 / 10 系统。
