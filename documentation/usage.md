# 使用方法

- [使用 `Shell`](#使用-Shell)

- [使用 `Assistant`](#使用-Assistant)

- [预置功能](#预置功能)

- [转发文件](#转发文件)

- [附加参数](#附加参数)

- [用户输入](#用户输入)

- [配置文件](#配置文件)

- [外部程序](#外部程序)

	- [执行规则](#执行规则)

	- [系统限制](#系统限制)

	- [程序清单](#程序清单)

## 使用 `Shell`

通过 `Shell` 可以在系统终端中以命令行的形式使用工具。

需要在启动应用时传递命令参数，格式如下：

* `kernel` : *`string`*

  内核文件路径。

* `script` : *`string`*

  脚本文件路径。也可以 `$` 为首字符标识，指示字符串的余下内容作为 JS 脚本。

* `argument` : *`string...`*

  执行参数。

> 若执行过程无错误发生，则 main 函数返回值为 `0` ，否则为 `1` 。

## 使用 `Assistant`

`Assistant` 基于 `Kernel` 、`Script` 等模块实现了多种子功能模块。

可以通过命令行参数传递命令，这在 `Windows` 、`Linux` 、`Macintosh` 系统中可用：

* ***`application`***

  将首个参数设为该值，以指示应用读取并应用余下参数作为命令。

  否则，应用将静默地忽略所有参数，不会执行任何命令。

* **`-launch`**

  启动新标签页。

	* `title` : *`string`*

	  页标题。

	* `type` : *`string`*

	  模块类别。

	* `option` : *`string...`*

	  模块参数。

* **`-forward`**

  转发资源至内部模块。

	* `resource` : *`string...`*

	  资源项。

> 通过命令行参数启动应用时，将始终创建新的应用实例。

也可通过应用链接传递命令，这在 `Windows` 、`Macintosh` 、`Android` 、`Iphone` 系统中可用：

* ***`com.twinstar.twinning.assistant:/application?`***

	* `command` : *`string...`*

	  命令参数。可以多次指定，所有查询值被视作字符串数组。

> 通过应用链接启动应用时，只会在不存在应用实例时才会创建新的应用实例，如果系统中已存在的应用实例，会将其切换至前台并应用新的命令参数。

应用还提供了文件的转发器扩展支持，这在以下系统中可用：

* `Windows`

  如果启用了转发器扩展，可以在 `Explorer` 的文件上下文菜单中看到 `⌈ Twinning Assistant ⌋` 选项。

* `Macintosh`

  如果启用了转发器扩展，可以在 `Finder` 的文件上下文菜单中看到 `⌈ Twinning Assistant ⌋` 选项。

* `Android`

  如果启用了转发器扩展，可以在系统或第三方的文件管理器的文件共享列表中看到 `⌈ Twinning Assistant ⌋` 选项。

  > 注意：由于 Android 系统的限制，应用无法直接获取所转发文件的绝对路径，具体参见 [Android Content URI 处理方式](./question.md#Android-Content-URI-处理方式) 。

* `Iphone`

  如果启用了转发器扩展，可以在系统或第三方的文件管理器的文件共享列表中看到 `⌈ Twinning Assistant ⌋` 选项。

应用提供以下功能模块：

* `Task Worker`

  该模块模拟了 `Shell` 的控制台 UI 。

  > `Kernel` 、`Script` 、`Argument` 参数需要在模块设置中定义。

* `Command Sender`

  该模块配合 `Task Worker` 使用，能够直观地编辑想要使用的功能与参数。

  > 功能在模块配置文件中定义。

* `Resource Shipper`

  该模块配合 `Task Worker` 使用，能够方便地转发文件并选择想要使用的功能与参数。

  > 功能在模块配置文件中定义。

* `PopCap Animation Viewer`

  该模块能够查看 PopCap Animation (PAM) 动画文件。

  > 将解码 `*.pam` 文件所得的 `*.pam.json` 文件与动画中引用到的纹理分解图 `*.png` 放置在同一目录内，并在页面中导入目录或 `*.pam.json` 文件即可查看动画。

* `Kairosoft Game Manager`

  该模块能够方便地查看与修改系统中安装的开罗游戏。

## 预置功能

工具提供的预置功能一般接受一个文件或目录作为输入，并在处理完成后将数据输出至另一个文件或目录，默认情况下将输出至同级目录中。功能一般分为两种：

* 常规功能

  处理单项事务，例如将单个 PopCap RTON 文件解码为 JSON 文件。

* 批处理功能

  对某种特定常规功能的包装，接受一个目录作为输入并处理其中的所有匹配文件，例如将一个目录内的所有 PopCap RTON 文件解码为 JSON 文件；同时也对一些需要频繁申请与释放内存的常规功能进行了效率优化。

  批处理功能的所需参数与其常规版本保持一致，但标识符附加了 `!batch` 后缀用以区分。

各功能根据性质分为多个功能组，并存放于 `<home>/script/executor/implementation` 。

具体的功能与其参数可查看对应的脚本文件或 Command Sender 模块配置文件 `<home>/assistant/core_command_sender.json` 。

## 转发文件

工具被设计为主要处理外存空间中的文件对象，术语 ⌈ **转发** ⌋ 是指以文件对象的路径作为附加参数以启动工具。可以通过命令行传参等方式进行转发。

将需要处理的文件对象转发给工具，工具会根据该文件对象的类型列出可用功能，用户输入需要执行的功能序号即可。

> 注意：如果向工具转发了一个目录，用户将在可选功能列表中看到诸多以 `[*]` 作为前缀的功能，它们是批处理功能，会依次处理目录中的每一个子文件与子目录。大部分单一输入且单一输出的功能都具备批处理版本。
>
> 常见的误区是用户在需要对某个目录执行常规功能时，错误选用了批处理功能。例如，如果需要将一个 `.rsb.bundle` 目录打包为 `rsb` 文件，那么需要选择常规版本的 `PopCap Resource-Stream-Bundle Pack` 而非带 `[*]` 前缀的批处理版本。

## 附加参数

用户可以在启动工具时传入附加参数，若未提供附加参数，则会在运行时要求用户输入。附加参数格式如下：

* `command`

  命令项，可以多次指定，工具会依次处理。

	* `input` : *`string`*

	  指定了命令的输入数据，即一个文件或目录的路径，作为功能的输入参数。

	  如果输入值为 `?none` ，则输入值为 `null` ，无输入参数的功能只有在输入为 `null` 时才会出现在候选中。

	* **`-method`** : *`string`*

	  指定需执行的功能，后跟该功能的标识符。

	  如果输入值为 `?filtered` ，将根据输入值来筛选出可用的功能，并列出给用户候选。

	  如果输入值为 `?unfiltered` ，将列出所有功能给用户候选。

	* **`-argument`** : *`string`*

	  指定需要传给功能的参数，后跟一个 JSON 字符串，且必须可解析为一个 `Object` 。

> 例 - 使用工具解码桌面上的 `test.pam` 文件：
>
> `> .\launch.ps1 "C:\Users\TwinStar\Desktop\test.pam" "-method" "popcap.animation.decode"`
>
> 该命令以 `test.pam` 文件的路径作为输入参数，并指定需执行的功能为 `PopCap Animation 解码` 。执行完毕后，可在桌面看到一个名为 `test.pam.json` 的新文件，即为解码后的 PAM 数据。

## 用户输入

工具运行时会向用户输出消息，或请求用户输入一些参数，输入值类型及格式如下：

> 如果输入的文本为空，则视作输入了空值；\
> 如果输入的文本以 `?` 起始，则视作宏指令，工具会根据指令与上下文计算出实际输入；
> 特别地，如果输入的文本以 `??` 起始，将截取其后的文本作为实际输入，通过这种方式可以输入空字符串而非空值；
> 所有类型的输入都支持 `?terminate` 宏指令，输入后会强制终止当前脚本的运行。

* `Pause` 暂停

  暂停程序等待用户响应。

* `Boolean` 布尔值

  单个字符 `y` 或 `n` ，表示 ⌈ 是 ⌋ 与 ⌈ 否 ⌋ 。

* `Integer` 整数

  一个十进制整数，不可包含小数点。

* `Floater` 浮点数

  一个十进制数，可包含小数点。

* `String` 字符串

  一行文本。

  > 支持如下宏：\
  > `?e/empty` 空字符串。

* `Size` 尺寸

  一个无符号十进制数，后跟随一个二进制单位（ b = 2^0 , k = 2^10 , m = 2^20 , g = 2^30 ），用于表示存储容量。

  > 例如 `4k` 表示 4096 字节的存储容量。

* `Path` 路径

  可用于本地文件系统的路径，分为输入路径与输出路径，前者指向磁盘上一个已存在的文件或目录的路径，后者指向磁盘上一个不存在的路径。

  > 如果输入路径由一对单引号或双引号包围，工具将自动去除引号；\
  > 如果输入了一段相对路径，则该路径是相对于工具的工作目录 `<home>/workspace` 计算的。
  >
  > 支持如下宏：\
  > `?g/generate` 通过所给路径生成可用输出路径（附加后缀）；\
  > `?m/move` 移动已有文件；\
  > `?d/deltet` 删除已有文件；\
  > `?o/overwrite` 覆写已有文件。

* `Enumeration` 枚举

  多个可选项中的一项。

## 配置文件

工具的脚本配置目录 `<home>/script/configuration` 中存储了脚本运行时需要的配置项，用户可以自行修改配置文件以改变脚本行为。

`<home>/script/configuration/language` 目录内存储了脚本使用到的多语言文本，可以修改或创建新的语言文本文件以改变脚本的交互文本。

`<home>/script/configuration/setting.json` 是脚本设置文件，它的配置项如下。

* `<configuration>`

	* `byte_stream_use_big_endian` : `boolean` = `false`

	  内部字节流操作时使用大端序。这个选项对一些大端序文件的处理是必要的。默认禁用，因为大多数时候用户处理的都是小端序文件。

	* `common_buffer_size` : `string` = `64.0m`

	  公用缓存区大小。用于编码 JSON 字符串、编码 PNG 图像等，如果编码时所需的内存用量超过这一大小，工具将处理失败。

	* `json_format_disable_array_trailing_comma` : `boolean` = `false`

	  输出 JSON 时禁用数组尾随逗号。

	* `json_format_disable_array_line_breaking` : `boolean` = `false`

	  输出 JSON 时禁用数组换行。

	* `json_format_disable_object_trailing_comma` : `boolean` = `false`

	  输出 JSON 时禁用对象尾随逗号。

	* `json_format_disable_object_line_breaking` : `boolean` = `false`

	  输出 JSON 时禁用对象换行。

	* `thread_limit` : `bigint` = `0`

	  线程池上限数。目前无实际作用。

	* `external_enable_android_termux` : `boolean` = `true`

	  在 `Android` 上调用外部程序时，将调用 `Termux` 环境中的程序，而非系统原生的程序。

	* `external_program_path` : `Record<string, null | string>` = `{ ... }`

	  指定可能会调用的外部程序的路径，若为 `null` ，则将在运行时检索 `PATH` 环境变量。

	* `android_temporary_directory_for_data` : `string` = `/data/local/tmp/twinning`

	  指定在 `Android` 上调用外部程序时，可能需要创建的临时目录的路径。

	* `android_temporary_directory_for_sdcard` : `string` = `/sdcard/.twinning`

	  指定在 `Android` 上调用外部程序时，可能需要创建的临时目录的路径。

	* `language` : `string` = `english`

	  交互语言。可以为 `english` 或 `chinese` 或 `vietnamese` 。

	* `executor_typical_method_disable_name_filter` : `boolean` = `false`

	  禁用命令处理器的名称过滤行为。默认情况下，工具会对命令输入的文件路径进行扩展名匹配，不匹配的功能将不对用户显示；禁用后将显示所有功能；同时也会影响批处理功能的名称过滤行为，若禁用，批处理功能将不再跳过名称不匹配的文件。

	* `executor_typical_method_configuration` : `boolean` = `false`

	  各项功能的名称过滤规则与参数默认值。可以修改以符合自己的使用习惯。

	  过滤规则配置将会对该功能的输入值生效，如 `*.rsb` 表示只有在输入文件的扩展名为 `rsb` 时才显示该功能。

	  参数值为字符串 `?input` 时，将在执行时要求用户输入参数。

	  有些参数可以由程序自动生成，用户可以指定参数值为字符串 `?automatic` 以启用自动生成行为，文档中将以 `~` 做出标识，并在其后跟随自动生成的值。通常来说，输出参数能够以输入参数的值自动生成，例如 `popcap.animation.decode` 功能以 `*.pam` 文件作为输入参数，并自动生成同名的 `*.pam.json` 文件作为输出参数。

	* `executor_popcap_pvz2_resource_convert_ptx_format_map_list` : `...` = `...`

	  特定功能需要使用的配置数据。用于 `popcap.pvz2.resource_stream_bundle.resource_convert` 。

	* `executor_popcap_pvz2_package_project_conversion_setting` : `...` = `...`

	  特定功能需要使用的配置数据。用于 `popcap.pvz2.package_project.parse` 。

	* `command_notification_time_limit` : `null | bigint` = `15000`

	  命令通知时限。在某项命令执行完成后，如果有效执行时间超过该值（毫秒），将推送系统通知以提醒用户。设为 `null` 将禁用通知。

## 外部程序

工具的某些功能会调用外部程序，需要用户自行下载与安装所需环境。

### 执行规则

在需要调用外部程序时，工具会通过一个特定的别名 `<alias>` 在系统中检索对应的可执行程序或脚本，遵循以下规则：

* 检索配置文件中 `external_program_path` 列表中是否为别名指定了程序路径，如果存在，则不再尝试继续检索。

* 否则，遍历 `PATH` 环境变量，检索与别名匹配的文件。

* 首先，匹配文件名与别名完全一致的文件 `<alias>` 。

* 其次，匹配携带可执行文件扩展名的文件 `<alias>.exe`（对于 `Windows` ）。

* 再次，匹配携带脚本文件扩展名的文件（按优先级依次为）`<alias>.sh` 、`<alias>.ps1` 、`<alias>.cmd|bat`（对于 `Windows` ）。

对于检索到的目标程序，将按照以下规则执行：

* 如果目标程序是可执行文件，将直接创建对应进程。

* 如果目标程序是脚本文件，将调用对应的解释器执行它们，解释器程序也会通过上述流程进行检索，但只会匹配可执行文件，不会尝试作为脚本调用其他解释器来执行。

  > 对于 `sh` 脚本，将调用 `sh` 解释器执行。\
  > 对于 `ps1` 脚本，将调用 `pwsh` 解释器执行。\
  > 对于 `cmd` 或 `bat` 脚本，将调用 `cmd` 解释器执行。

用户需要安装所需外部程序并正确配置 `PATH` 环境变量（或在配置文件中指定），以确保工具能正确检索到目标程序。

### 系统限制

由于 Android 的系统限制，应用在调用外部程序时可能会受到 `SELinux` 的限制，即使授予了文件可执行权限，也无法成功执行，这一般会表现为返回错误码 `127` 。

若要让应用调用非系统内建的外部程序，需要满足以下几点要求：

* 设备已获取 `ROOT` 权限。

* 通过 `setenforce 0` 指令将 `SELinux` 切换到不安全的宽容模式。

* 将可执行程序文件放置于非 FUSE 目录内，确保应用有权访问，并赋予可执行权限。

此外，在一些系统环境中，应用默认的程序检索逻辑会优先匹配到 `/vendor/bin` 目录内的供应商实现程序，例如 `sh` ，它们往往会导致预期之外的结果。可以在 `<home>/script/configuration/setting.json` 中指定所需程序的其绝对路径。

不过，在 `Android` 上执行外部程序时，会默认调用 `Termux` 环境而非本机环境，这种情况下无需上述权限要求，只需为应用授予调用 `Termux` 服务的权限。

### 程序清单

以下说可能被工具调用的外部程序：

* [sh](#)

* [openssl](https://www.openssl.org/)

* [7z](https://7-zip.org/)

* [WwiseConsole](https://www.audiokinetic.com/en/download)

  > `WwiseConsole` 可执行程序的路径参见 [官方文档](https://www.audiokinetic.com/en/public-library/?source=SDK&id=bankscommandline.html) 。

  > 建议使用 `2019.2` 版本，否则可能无法成功完成所需调用；此外，在编码 `AAC` 格式的音频时，该程序还会调用 [QuickTime](https://support.apple.com/kb/DL837) `>=7.6` 。

* [vgmstream-cli](https://vgmstream.org/)

* [Il2CppDumper](https://github.com/Perfare/Il2CppDumper)

* [signtool](https://learn.microsoft.com/en-us/windows/win32/seccrypto/signtool)

* [mt](https://learn.microsoft.com/en-us/windows/win32/sbscs/mt-exe)

* [makepri](https://learn.microsoft.com/en-us/windows/uwp/app-resources/compile-resources-manually-with-makepri)

* [makeappx](https://learn.microsoft.com/en-us/windows/msix/package/create-app-package-with-makeappx-tool)

* [appimagetool](https://github.com/AppImage/appimagetool)

* [codesign](#)

* [security](#)

* [createdmg](https://github.com/create-dmg/create-dmg)

* [zipalign](https://developer.android.com/tools/zipalign)

* [apksigner](https://developer.android.com/tools/apksigner)

* [adb](https://developer.android.com/tools/adb)
