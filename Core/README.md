# TwinStar ToolKit - Core

核心，负责内部功能的实现。

本项目是跨平台的 `C++` 项目，不依赖其他模块。

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

| 库                                                                   | 用途                           |
|:--------------------------------------------------------------------:|:------------------------------:|
| [mscharconv 1.2.3](https://github.com/iboB/mscharconv)               | charconv 支持 ( for non-MSVC ) |
| [fmt 9.1.0](https://github.com/fmtlib/fmt)                           | 字符串格式化                   |
| [tinyxml2 9.0.0](https://github.com/leethomason/tinyxml2)            | XML 读写                       |
| [md5 1.0](https://github.com/JieweiWei/md5)                          | MD5 哈希计算                   |
| [Rijndael](#)                                                        | Rijndael 加密与解密            |
| [zlib 1.2.13](https://github.com/madler/zlib)                        | ZLib 压缩与解压                |
| [bzip2 1.0.8](https://sourceware.org/bzip2/)                         | BZip2 压缩与解压               |
| [lzma 22.01](https://www.7-zip.org/sdk.html)                         | Lzma 压缩与解压                |
| [open-vcdiff 0.8.4](https://github.com/google/open-vcdiff)           | VCDiff 差异分析                |
| [avir 3.0](https://github.com/avaneev/avir)                          | 图像缩放                       |
| [ETCPACK 2.74](https://github.com/Ericsson/ETCPACK)                  | ETC1 解压                      |
| [etcpak 1.0](https://github.com/wolfpld/etcpak)                      | ETC1 压缩                      |
| [PVRTCCompressor](https://github.com/brenwill/PVRTCCompressor)       | PVRTC4 压缩与解压              |
| [libpng 1.6.39](http://www.libpng.org/pub/png/libpng.html)           | PNG 读写                       |
| [quickjs 2021-03-27](https://github.com/bellard/quickjs)             | JS 引擎                        |
| [quickjs 2021-03-27 (c-smile)](https://github.com/c-smile/quickjspp) | JS 引擎 ( for MSVC )           |
| [vld 2.5.1](https://github.com/KindDragon/vld)                       | 内存泄漏检测                   |

## 说明

* 关于接口
	
	1. 本项目被编译为动态库，需要调用方进行动态加载。
	
	2. 接口声明位于 `- <project>/core/interface/interface.hpp` 。
	
	3. 为了容许来自其他语言的调用，接口被设计为完全的 C 风格。输入参数的所有权归调用方，调用方需确保其能够正确析构；输出参数与返回值的所有权属于库自身，调用方不应对其进行读取以外的任何操作。

* 关于构建
	
	1. 由于项目中大量使用模板，并仅使用头文件进行代码组织，最终只有单个源文件被编译，这导致了较长的编译时间（clang 20m~ | msvc 80m~）与极高的内存占用（clang 12G~ | msvc 48G~），编译时需确保物理内存与交换内存充足，应考虑在未来使用 C++ Modules 进行重构（在其成熟之后）。
	
	2. 由于 GCC 的策略，需要添加 `-fpermissive` 选项以使 GCC 容许项目中的部分代码。
	
	3. 由于 GCC 的 BUG ，本项目无法通过 GCC 编译，参阅 [GCC Bugzilla # 102367](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=102367) 。

* 对第三方库的修改
	
	本项目对引用的第三方库做出了少许修改，如下：
	
	* `fmt`：修改浮点数输出格式。
	
	* `md5`：修改传参类型。
	
	* `Rijndael`：修正非标准代码，避免符号冲突。
	
	* `ETCPACK`：修正错误代码，移除平台依赖，暴露接口。
	
	* `PVRTCCompressor`：引入依赖头文件。
	
	* `quickjs`：暴露接口。
	
	> 任何修改都会以 `// TwinStar : source|insert|remove|change` 做出标示。
