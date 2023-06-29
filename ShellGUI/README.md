# TwinStar ToolKit - Shell GUI

外壳，提供图形界面。

本项目是跨平台的 `Flutter` 项目，依赖 `Core` 的实现。

## 项目开发

* 要求
	
	* [Flutter 3.10.5](https://docs.flutter.dev/get-started/install)

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
	
	`<core> <script> <argument>...`
	
	* `<core>`
		
		第一参数为核心文件路径。
	
	* `<script>`
		
		第二参数是传给核心处理逻辑的脚本文件路径。也可以 `$` 为首字符标识，指示字符串的余下内容作为 JS 脚本。
	
	* `<argument>...`
		
		剩余参数作为传给核心处理逻辑的参数。
	
	`-additional <argument>...`
	
	若首个命令行参数为 `-additional` ，则以应用设置中设定的默认命令启动应用，`<argument>...` 为附加参数。
	
	若传入命令行参数，则应用将在命令执行完毕且成功后自动退出（可以在应用设置内禁用此行为）。
	
	若不传入命令行参数，则直接启动应用，用户可以手动执行默认命令（可以在应用设置内设定默认命令）。
	
	> `Android` 与 `iPhone` 无法直接传入命令行参数；`Android` 可以通过 `Intent` 传入命令行参数 `action = "com.twinstar.toolkit.shell_gui.action.LAUNCH", extra = { "command": Array<String> }` 。

* 关于 Android 平台的必要说明
	
	对于 `Android 7+` 系统，存在动态库加载限制，应用只能加载 `default_library_paths` 与 `permitted_paths` 中的库文件，并且对于后者必须指定绝对路径而不允许只指定库名。
	
	因此，为了实现动态加载 Core 库文件的目的，应用会在每次运行时将指定的 Core 路径复制至 `/data/user/<user>/<package>/files` 中（这是 `permitted_paths` 中的一项，但不确定是否可用于大多数设备与系统）；并且在应用打包时也必须包含与 Core 编译时所用版本一致的 `libc++_shared.so` 文件。
	
	> 当前项目中包含的 libc++_shared.so 版本为 NDK r25c 。
	
	> 具体参阅 [Android 文档](https://source.android.com/docs/core/architecture/vndk/linker-namespace) 。
