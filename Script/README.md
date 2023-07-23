# TwinStar ToolKit - Script

脚本，控制工具的运行流程。

本项目是跨平台的 `TypeScript` 项目，依赖 `Kernel` 、`Shell` 、`Shell GUI` 的实现。

## 项目开发

* 要求
	
	* [Node.js 20.3](https://nodejs.org/)
	
	* [TypeScript 5.1](https://www.typescriptlang.org/)
	
	* [PowerShell](https://learn.microsoft.com/powershell/)

* 构建
	
	在 `+ <project>` 下执行 `> tsc` ，构建产物将输出至 `+ <project>/.build` 。

* 链接配置文件
	
	初次构建后，还须在 `+ <project>` 下以管理员权限执行 `> ./link.ps1` ，该脚本将在输出目录内创建指向脚本配置文件的软链接。
	
	> 该脚本仅适用于 `PowerShell` ，需要预先安装该环境。你也可以考虑自行编写 shell 脚本。

## 第三方库使用

| 库                                                                         | 用途                           |
|:--------------------------------------------------------------------------:|:------------------------------:|

## 说明
