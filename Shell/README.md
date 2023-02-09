# TwinStar ToolKit - Shell

外壳程序，提供基于终端的命令行界面。

本项目是跨平台的 `C++` 项目，依赖 `Core` 的实现。

## 项目开发

* 要求
	
	* 项目构建工具：[xmake 2.7.3+](https://xmake.io/#/) 。
	
	* 本地编译工具链：[MSVC 19.33+ with MSVC-STL and Windows-SDK for Windows](https://visualstudio.microsoft.com/downloads/) 、[Clang 14+ with libstdc++/libc++ and POSIX for Linux/Macintosh](https://llvm.org/) 。
	
	* 跨平台编译工具链：[NDK 25.1+ for Android](https://developer.android.com/ndk/downloads) 。

* 开发
	
	通过以下命令，可以在 `+ <project>/.project` 内生成其他开发工具所使用的项目文件，以便于在其他开发工具内开发项目。
	
	* MSBuild ：`> xmake project -k vsxmake .project` ，适用于 VS 。
	
	* CMake ：`> xmake project -k cmakelists .project` ，适用于 VS 、CLion 。
	
	> 具体参阅 [xmake 文档](https://xmake.io/#/plugin/builtin_plugins?id=generate-ide-project-files) 。

* 构建
	
	在 `+ <project>` 下执行 `> xmake f -o .build` & `> xmake` ，构建产物将输出至 `+ <project>/.build` 。
	
	> 具体参阅 [xmake 文档](https://xmake.io/#/) 。

## 第三方库使用

| 库                                                                   | 用途                           |
|:--------------------------------------------------------------------:|:------------------------------:|

## 说明
