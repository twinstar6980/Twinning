# TwinStar ToolKit - Shell

外壳，提供命令行界面。

本项目是跨平台的 `C++` 项目，依赖 `Core` 的实现。

## 项目开发

* 要求
	
	* [xmake 2.7.8+](https://xmake.io/#/)
	
	* [MSVC 19.35+ with MSVC-STL and Windows-SDK for Windows](https://visualstudio.microsoft.com/downloads/)
	
	* [Clang 16.0+ with libstdc++ and POSIX for Linux](https://llvm.org/)
	
	* [Clang 16.0+ with libc++ and POSIX for Macintosh](https://llvm.org/)
	
	* [NDK 25.2+ for Android](https://developer.android.com/ndk/downloads)
	
	* [Clang 16.0+ with libc++ and POSIX for iPhone](https://llvm.org/)

* 开发
	
	通过以下命令，可以在 `+ <project>/.project` 内生成其他开发工具所使用的项目文件，以便于在其他开发工具内开发项目。
	
	* MSBuild ：`> xmake project -k vsxmake .project` ，适用于 VS 。
	
	* CMake ：`> xmake project -k cmakelists .project` ，适用于 VS 、CLion 。
	
	> 具体参阅 [xmake 文档](https://xmake.io/#/plugin/builtin_plugins?id=generate-ide-project-files) 。

* 构建
	
	在 `+ <project>` 下执行 `> xmake f -o .build` & `> xmake` ，构建产物将输出至 `+ <project>/.build` 。
	
	> 具体参阅 [xmake 文档](https://xmake.io/#/) 。

## 第三方库使用

| 库                                                                       | 用途                           |
|:------------------------------------------------------------------------:|:------------------------------:|
| [tinyfiledialogs 3.10](https://sourceforge.net/projects/tinyfiledialogs) | 系统 UI 功能                   |

## 说明

* 命令行参数
	
	`<core> <script> <argument>...`
	
	* `<core>`
		
		第一参数为核心文件路径。
	
	* `<script>`
		
		第二参数是传给核心处理逻辑的脚本。该参数是一段表示 JS 脚本的字符串，或以 `?` 为首字符作为标识的脚本文件路径。
	
	* `<argument>...`
		
		剩余参数作为传给核心处理逻辑的参数。
	
	若执行过程无错误发生，则 main 函数返回值为 0 ，否则为 1 。
