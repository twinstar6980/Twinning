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

## 第三方库使用

| 库                                                                         | 用途                           |
|:--------------------------------------------------------------------------:|:------------------------------:|
| [tinyfiledialogs 3.17.5](https://sourceforge.net/projects/tinyfiledialogs) | 系统 UI 功能                   |

## 说明

* 对第三方库的说明
	
	* `tinyfiledialogs`：将版本停留在 `3.17.5` ，因为后续版本中 `tinyfd_notifyPopup` 函数的行为变更，无法实现推送系统级通知的需求。
