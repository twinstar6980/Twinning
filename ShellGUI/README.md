# TwinStar ToolKit - Shell GUI

外壳，提供图形界面。

本项目是跨平台的 `Flutter` 项目，依赖 `Kernel` 的实现。

## 项目开发

* 要求
	
	* [Flutter 3.13](https://docs.flutter.dev/get-started/install)

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

* 命令行参数
	
	`-additional_argument <additional_argument>...`
	
	* `<additional_argument>...`
		
		传给内核处理逻辑的附加参数。
	
	若传入命令行参数，则应用将在启动后自动启动控制台。
	
	> `Android` 与 `iPhone` 无法直接传入命令行参数；`Android` 可以通过 `Intent` 传入命令行参数 `action = "com.twinstar.toolkit.shell_gui.action.LAUNCH", extra = { "command": Array<String> }` 。
	
	> 内核文件路径、脚本文件路径、参数等选项需在应用内设置页中预先设定。

* 关于 Android 平台的必要说明
	
	对于 `Android 7+` 系统，存在动态库加载限制，应用只能加载 `default_library_paths` 与 `permitted_paths` 中的库文件，并且对于后者必须指定绝对路径而不允许只指定库名。
	
	因此，为了实现动态加载 Kernel 库文件的目的，应用会在每次运行时将指定的库文件复制至 `/data/user/<user>/<package>/cache` 中（该目录位于 `permitted_paths` 下，但不确定是否可用于大多数设备与系统）；并且在应用打包时也必须包含与 Kernel 编译时所用版本一致的 `libc++_shared.so` 文件。
	
	> 当前项目中包含的 libc++_shared.so 版本为 NDK 26.0 。
	
	> 具体参阅 [Android 文档](https://source.android.com/docs/core/architecture/vndk/linker-namespace) 。
