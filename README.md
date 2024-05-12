# TwinStar ToolKit

`TwinStar ToolKit` aka `twins` 。

本工具主要用于处理 PopCap 游戏（特别是 PvZ-1 、PvZ-2 ）中的资源文件，以使玩家能够更加容易地对这些游戏进行 MOD 创作。

## 开源说明

本工具是自由软件并遵循 **GPL v3** 协议，任何人都可以自由地使用与修改，但不应以非自由的形式重新分发。

## 模块组成

* `Kernel`
	
	内核，负责内部功能的实现。

* `Shell`
	
	外壳，提供基本的用户界面。

* `Script`
	
	脚本，控制工具的运行流程。

* `Forwarder`
	
	转发器，将快捷选项注入系统级文件分享接口。

* `Assistant`
	
	助理者，实现了额外的高级辅助功能。

* `Assistant Plus`
	
	助理者+，面向 Windows 平台的特化版本。

## 平台支持

* 操作系统：`Windows 7+` 、`Linux ~` 、`Macintosh 13~` 、`Android 9+` 、`Iphone 16~` 。

* 处理器架构：`x86 32|64` 、`arm 32|64` 。

## 鸣谢

* [`icons8.com`](https://icons8.com) 工具的图标来源于该网站。

* `时漪` `PAK向日葵` `伊特` 等于 2019 年与我一同破解 `PopCap Animation (aka PAM)` 格式。

* [`迎风听雨`](https://github.com/YingFengTingYu) 为项目中诸多 PopCap 格式的解析提供了帮助，包括 `Crypt-Data` 、`U-Texture` 、`Sexy-Texture` 、`Re-Animation` 、`Particle` 、`Trail` 、`Effect` 、`Character-Font-Widget-2` 。

* `Xentax` 论坛上一份关于 `Wwise Sound-Bank` 格式的 [文档](http://wiki.xentax.com/index.php/Wwise_SoundBank_(*.bnk)#STID_section) ，帮助我初步了解了该文件格式。

* `1Zulu` 、`黄宠红花` 等在 PvZ-2 早期对 `RTON` 文件进行研究与普及的人士，他们为包括我在内的许多早期 MODDER 提供了启蒙，PvZ-2 MOD 的发展离不开他们的贡献。

本工具的完善得益于对 PvZ-2 MOD 技术进行普及的人们。我始终认为，只有技术开放，才能让绝大多数人受益，才能将 MODDER 们从繁重的、与创意无关的工作中解放，才能实现生产力的进步；而技术封闭则反之，妄图将技术私有、垄断，是不切实际的，同时也阻碍着垄断者自身的生产力进步。

如果你经历那个 MODDER 们必须手工对 RTON 文件进行二进制层面的编辑的时代，经历过那个 MODDER 们必须依赖 “可修改数据包” 与 “十六进制编辑器” 进行资源数据修改的时代，你会知道现在的一切是多么来之不易，而这，是技术开放带给我们的。
