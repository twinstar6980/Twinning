# Shell

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
	
	* `MSBuild` for VS
		
		`> xmake project -k vsxmake .project`
	
	* `CMake` for VS and CLion
		
		`> xmake project -k cmakelists .project`
	
	> 具体参阅 [xmake 文档](https://xmake.io/#/plugin/builtin_plugins?id=generate-ide-project-files) 。

* 构建
	
	通过以下命令，可以在 `+ <project>/.build` 内生成构建产物。
	
	* `Windows`
		
		`> xmake f -o .build -m release -p windows -a x64 --toolchain=msvc`
	
	* `Linux`
		
		`> xmake f -o .build -m release -p linux -a x86_64 --toolchain=clang-18`
	
	* `Macintosh`
		
		`> xmake f -o .build -m release -p macosx -a arm64 --cc=clang-18 --cxx=clang-18 --xcode_sdkver=13.3 --target_minver=13.3`
	
	* `Android`
		
		`> xmake f -o .build -m release -p android -a arm64-v8a --ndk=<..> --ndk_sdkver=28 --runtimes=c++_shared`
	
	* `Iphone`
		
		`> xmake f -o .build -m release -p iphoneos -a arm64 --cc=clang-18 --cxx=clang-18 --xcode_sdkver=16.4 --target_minver=16.4`
	
	> 具体参阅 [xmake 文档](https://xmake.io/#/) 。

## 第三方库使用

| 库                                                                         | 用途                           |
|:--------------------------------------------------------------------------:|:------------------------------:|
| [tinyfiledialogs 3.18.1](https://sourceforge.net/projects/tinyfiledialogs) | 系统 UI 功能                   |

## 说明
