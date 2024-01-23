# TwinStar ToolKit - Shell GUI

外壳，提供图形界面。

本项目是跨平台的 `Flutter` 项目，依赖 `Kernel` 的实现。

## 项目开发

* 要求
	
	* [Flutter 3.16](https://docs.flutter.dev/get-started/install)

* 开发
	
	> 具体参阅 [Flutter 文档](https://docs.flutter.dev/get-started/editor) 。

* 构建
	
	在 `+ <project>` 下执行以下命令，构建产物将输出至 `+ <project>/build` 。
	
	* Windows-MSIX: `flutter pub run msix:create --release --architecture x64 --install-certificate false --certificate-path <..> --certificate-password <..>`
	
	* Android-APK: `flutter build apk --release --split-per-abi --target-platform android-arm64`
	
	> 具体参阅 [Flutter 文档](https://docs.flutter.dev/deployment/windows) 。

## 第三方库使用

> 参见 [pubspec.yaml](./pubspec.yaml) 。

## 说明

* 命令参数
	
	`[ -additional_argument <additional_argument>... ]`
	
	* `-additional_argument <additional_argument>...`
		
		附加参数。若指定该项，应用将在启动后自动启动控制台。
	
	> 内核文件路径、脚本文件路径、执行参数等选项需在应用内设置页中预先设定。
	
	> `Android` 与 `iPhone` 系统不支持命令行传参，须通过下文的应用链接传递启动参数。

* 应用链接
	
	`twinstar.toolkit.shell-gui:/run?command=<command>`
	
	* `command`
		
		命令参数。可以多次指定，所有查询值被视作字符串数组。
	
	> 仅在应用未启动时才可通过链接传参；若在应用已启动的状态下打开链接，应用会切换到前台，但不会接收新的命令参数。
	
	> 应用链接仅适用于 `Android` 与 `iPhone` 系统。

* 关于 Android 平台的必要说明
	
	自 `Android 7+` 起，系统会对应用的动态库加载行为加以限制，应用只能加载 `default_library_paths` 与 `permitted_paths` 中的库文件，并且对于后者必须指定绝对路径而不允许只指定库名。
	
	因此，为了实现动态加载 Kernel 库文件的需求，应用会在每次运行时将指定的库文件复制至 `/data/user/<user>/<package>/cache` 中（该目录位于 `permitted_paths` 之中）；并且在应用打包时也必须包含一份 `libc++_shared.so` 文件。
	
	> 当前项目中包含的 libc++_shared.so 版本为 NDK 26.1 。
	
	> 具体参阅 [Android 文档](https://source.android.com/docs/core/architecture/vndk/linker-namespace) 。
