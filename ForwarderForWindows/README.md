# TwinStar ToolKit - Forwarder For Windows

转发器，使用户可以通过 Windows Explorer 右键菜单将文件转发至工具。

本项目是面向 `Windows` 的 `C++` 项目，依赖 `Script` 的实现。

## 项目开发

* 要求
	
	* [xmake 2.7.9](https://xmake.io/#/)
	
	* [MSVC 19.36 with MSVC-STL and Windows-SDK for Windows](https://visualstudio.microsoft.com/downloads/)
	
	* [vcpkg](https://vcpkg.io)

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
| [wil](https://github.com/microsoft/wil)                                    | Windows API                    |

## 说明

* 打包 MSIX
	
	构建出的动态库文件还需打包为 MSIX 并安装。打包所需文件存放在 `+ <project>/package` 中，可使用 [MSIX Packaging Tool](https://learn.microsoft.com/windows/msix/packaging-tool/tool-overview) 或其他方式进行打包。
	
	每次构建成功后，会自动将构建出的动态库文件复制为 `- <project>/package/Asset/Library/implement.dll` ，以便后续打包。
