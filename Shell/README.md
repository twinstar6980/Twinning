# Shell

外壳，提供基本的用户界面。

本项目是跨平台的 `C++` 项目，依赖 `Kernel` 的实现。

## 项目开发

* 要求
	
	* [xmake 3.0](https://xmake.io/#/)
	
	* [LLVM 20.1 (MinGW-ucrt)](https://github.com/mstorsjo/llvm-mingw) for `Windows`
	
	* [LLVM 20.1](https://llvm.org/) for `Linux`
	
	* [LLVM 20.1 (SDK 15.5)](https://llvm.org/) for `Macintosh`
	
	* [LLVM 19.0 (NDK 28.1)](https://developer.android.com/ndk/downloads) for `Android`
	
	* [LLVM 20.1 (SDK 18.5)](https://llvm.org/) for `Iphone`

## 第三方库使用

| 库                                                                         | 用途                |
|:--------------------------------------------------------------------------:|:-------------------:|
| [tinyfiledialogs 3.17.5](https://sourceforge.net/projects/tinyfiledialogs) | 系统 UI 功能        |

## 说明

* 对第三方库的说明
	
	* `tinyfiledialogs`：将版本停留在 `3.17.5` ，因为后续版本中 `tinyfd_notifyPopup` 函数的行为变更，无法实现推送系统级通知的需求。
