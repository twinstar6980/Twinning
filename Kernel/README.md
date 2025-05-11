# Kernel

内核，负责内部功能的实现。

本项目是跨平台的 `C++` 项目，不依赖其他模块的实现。

## 项目开发

* 要求
	
	* [xmake 2.9](https://xmake.io/#/)
	
	* [Clang 19.1 & WinAPI](https://visualstudio.microsoft.com/downloads/) for `Windows`
	
	* [Clang 20.1 & POSIX](https://llvm.org/) for `Linux`
	
	* [Clang 20.1 & POSIX](https://llvm.org/) for `Macintosh`
	
	* [Clang 19.0 & POSIX (NDK 28.1)](https://developer.android.com/ndk/downloads) for `Android`
	
	* [Clang 20.1 & POSIX](https://llvm.org/) for `Iphone`

## 第三方库使用

| 库                                                                         | 用途                |
|:--------------------------------------------------------------------------:|:-------------------:|
| [mscharconv 1.2.3](https://github.com/iboB/mscharconv)                     | charconv 支持       |
| [fmt 11.1.4](https://github.com/fmtlib/fmt)                                | 字符串格式化        |
| [tinyxml2 11.0.0](https://github.com/leethomason/tinyxml2)                 | XML 读写            |
| [md5 1.0](https://github.com/JieweiWei/md5)                                | MD5 哈希计算        |
| [Rijndael ?](#)                                                            | Rijndael 加密与解密 |
| [zlib 1.3.1](https://github.com/madler/zlib)                               | ZLib 压缩与解压     |
| [bzip2 1.0.8](https://sourceware.org/bzip2/)                               | BZip2 压缩与解压    |
| [lzma 24.09](https://github.com/ip7z/7zip)                                 | Lzma 压缩与解压     |
| [open_vcdiff 0.8.4](https://github.com/google/open-vcdiff)                 | VCDiff 差异分析     |
| [avir 3.1](https://github.com/avaneev/avir)                                | 图像缩放            |
| [etcpak 2.0](https://github.com/wolfpld/etcpak)                            | ETC1 压缩           |
| [PVRTCCompressor ?](https://github.com/brenwill/PVRTCCompressor)           | PVRTC4 压缩与解压   |
| [libpng 1.6.47](https://github.com/pnggroup/libpng)                        | PNG 读写            |
| [quickjs_ng 0.9.0](https://github.com/quickjs-ng/quickjs)                  | JS 引擎             |
| [vld 2.5.1](https://github.com/KindDragon/vld)                             | 内存泄漏检测        |

## 说明

* 关于接口
	
	1. 本项目被编译为动态库，需要调用方进行动态加载。
	
	2. 接口声明位于 `<project>/kernel/interface/interface.hpp` 。
	
	3. 为了允许来自不同语言的调用，接口被设计为完全的 `C` 风格。输入参数的所有权归调用方，调用方需确保其能够正确析构；输出参数与返回值的所有权属于库自身，调用方不应对其进行读取以外的任何操作。
	
	4. 接口具备线程安全性，但不建议在多个线程中同时调用同一库实例的接口，因为内部使用的脚本引擎 `quickjs_ng` 只能被不同线程互斥地调用；如需在多线程中调用接口，请为每个线程加载独立的库实例。

* 关于构建
	
	1. 本项目无法通过 GCC 与高版本 MSVC（17.11.5|19.41.34123+）的编译，这是由于 GCC 与 MSVC 在处理几处较为复杂的代码（特别是模板）时出现了 BUG 。请使用 Clang|ClangCL & LIBC++|MSVCSTL 进行编译。

* 对第三方库的说明
	
	本项目对引用的第三方库做出了少许修改，如下：
	
	* `md5`：修改接口的参数类型。
	
	* `Rijndael`：修正非标准代码，避免符号冲突。
	
	* `open_vcdiff`：修正非标准代码。
	
	* `etcpak`：移除不需要的依赖，暴露所需的接口。
	
	> 任何修改都会以 `// TwinStar : insert|remove|change` 做出标示。
