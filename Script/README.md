# TwinKleS ToolKit - Script

与 Core 及 Shell 配合使用的脚本。

此为跨平台 TypeScript 项目。

## 项目构建

1. 首先，安装 ⌈ node.js ⌋ 及 ⌈ typescript ⌋ 。

2. 之后，进入项目目录，执行 ⌈ tsc ⌋ ，将编译项目至输出目录 ⌈ .build ⌋ 内。

3. 初次编译后，还需运行 ⌈ link_config.bat ⌋ ，以在输出目录内创建指向脚本配置文件的符号链接。

4. 最后，删除工具主目录内的脚本目录 ⌈ `<home>/script` ⌋ ，并创建指向项目输出目录的符号链接 ⌈ `<home>/script` ⌋ ，即可开始进行拓展开发。
