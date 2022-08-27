# TwinKleS ToolKit - Windows Explorer Extension

适用于 Windows 的资源管理器拓展，将 ToolKit 集成至系统资源管理器的右键菜单中。

此为面向 Windows 的 C++ 项目。

## 系统要求

Windows 11 / 10 系统。

## 项目构建

本项目为 VS 2022 项目。

需要注意的是：项目中的 ./package/Assets/Library/implement.dll 实际上是一个指向 /.build/x64-Release/implement/implement.dll 的符号链接，在首次构建时，应运行 /package/Assets/Library/link.bat 文件以创建这一符号链接，并在每次更新 implement 子项目后重新构建（而非直接构建） package 子项目，从而使 VS 可将新的 implement.dll 打包进 msix 。

## 程序安装

进入 ⌈ Releases ⌋ 页，下载最新发布的版本，得到以下两个文件：

+ package_&lt;version&gt;_x64.cer
+ package_&lt;version&gt;_x64.msixbundle

首先，双击证书文件 .cer ，在弹出的窗口中依次选择 ⌈ 安装证书 ⌋ - ⌈ 本地计算机 ⌋ - ⌈ 将所有证书都放入下列存储 ⌋ - ⌈ 受信任的根证书颁发机构 ⌋ ，完成证书的安装。

接着，双击安装包文件 .msixbundle ，点击安装即可。

现在，你可以在任意文件或目录的右键菜单内看到 ⌈ TwinKleS ⌋ 选项。

如果没有看到该选项，请尝试重启资源管理器 explorer.exe ，或重启计算机。
