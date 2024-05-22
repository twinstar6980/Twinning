# TwinStar ToolKit - Shell

外壳，提供基本的用户界面。

本项目是跨平台的 `C++` 项目，依赖 `Kernel` 的实现。

## 项目开发

* 要求
	
	* [xmake 2.9](https://xmake.io/#/)
	
	* [MSVC 19.40 & WinAPI](https://visualstudio.microsoft.com/downloads/) for `Windows`
	
	* [Clang 18.1 & POSIX (libstdc++)](https://llvm.org/) for `Linux`
	
	* [Clang 18.1 & POSIX](https://llvm.org/) for `Macintosh`
	
	* [Clang 17.0 & POSIX (NDK 26.3)](https://developer.android.com/ndk/downloads) for `Android`
	
	* [Clang 18.1 & POSIX](https://llvm.org/) for `Iphone`

* 开发
	
	通过以下命令，可以在 `+ <project>/.project` 内生成其他开发工具所使用的项目文件，以便于在其他开发工具内开发项目。
	
	* MSBuild ：`> xmake project -k vsxmake .project` ，适用于 VS 。
	
	* CMake ：`> xmake project -k cmakelists .project` ，适用于 VS 、CLion 。
	
	> 具体参阅 [xmake 文档](https://xmake.io/#/plugin/builtin_plugins?id=generate-ide-project-files) 。

* 构建
	
	在 `+ <project>` 下执行 `> xmake f -o .build` & `> xmake` ，构建产物将输出至 `+ <project>/.build` 。
	
	> 具体参阅 [xmake 文档](https://xmake.io/#/) 。

## 第三方库使用

| 库                                                                         | 用途                           |
|:--------------------------------------------------------------------------:|:------------------------------:|
| [tinyfiledialogs 3.18.1](https://sourceforge.net/projects/tinyfiledialogs) | 系统 UI 功能                   |

## 说明
