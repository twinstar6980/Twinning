/**
 * JavaScript interface of Kernel
 * @version 72
 */
declare namespace Twinning.Kernel {

	// ------------------------------------------------

	/** 内部类型定义模板 */
	class T {

		// ------------------------------------------------

		/** 不存在的属性，用于避免TS类型兼容规则对内部类型的影响 */
		private _T: any;

		// ------------------------------------------------

		/* 通用操作 */

		/** 默认 构造 */
		static default(): T;

		/** 复制 构造 */
		static copy(it: T): T;

		// ------------------------------------------------

		/** JS值 操作 */

		/** JS值 类型 */
		static Value: any;

		/** JS值 构造 */
		static value(it: typeof T.Value): T;

		/** JS值 getter */
		get value(): typeof T.Value;

		/** JS值 setter */
		set value(it: typeof T.Value);

		// ------------------------------------------------

		/** 其他 */

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 无值 */
	type Void = void;

	// ------------------------------------------------

	/** 布尔值 */
	class Boolean {

		// ------------------------------------------------

		private _Boolean;

		// ------------------------------------------------

		static default(): Boolean;

		static copy(it: Boolean): Boolean;

		// ------------------------------------------------

		static Value: boolean;

		static value(it: typeof Boolean.Value): Boolean;

		get value(): typeof Boolean.Value;

		set value(it: typeof Boolean.Value);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 32位无符号整数 */
	class IntegerU32 {

		// ------------------------------------------------

		private _IntegerU32;

		// ------------------------------------------------

		static default(): IntegerU32;

		static copy(it: IntegerU32): IntegerU32;

		// ------------------------------------------------

		static Value: bigint;

		static value(it: typeof IntegerU32.Value): IntegerU32;

		get value(): typeof IntegerU32.Value;

		set value(it: typeof IntegerU32.Value);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 表征尺寸的整数，表现为64位无符号整数 */
	class Size {

		// ------------------------------------------------

		private _Size;

		// ------------------------------------------------

		static default(): Size;

		static copy(it: Size): Size;

		// ------------------------------------------------

		static Value: bigint;

		static value(it: typeof Size.Value): Size;

		get value(): typeof Size.Value;

		set value(it: typeof Size.Value);

		// ------------------------------------------------

	}

	/** Optional包装的Path */
	class SizeOptional {

		// ------------------------------------------------

		private _SizeOptional;

		// ------------------------------------------------

		static default(): SizeOptional;

		static copy(it: SizeOptional): SizeOptional;

		// ------------------------------------------------

		static Value: null | bigint;

		static value(it: typeof SizeOptional.Value): SizeOptional;

		get value(): typeof SizeOptional.Value;

		set value(it: typeof SizeOptional.Value);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 字符串 */
	class String {

		// ------------------------------------------------

		private _String;

		// ------------------------------------------------

		static default(): String;

		static copy(it: String): String;

		// ------------------------------------------------

		static Value: string;

		static value(it: typeof String.Value): String;

		get value(): typeof String.Value;

		set value(it: typeof String.Value);

		// ------------------------------------------------

	}

	/** Optional包装的String */
	class StringOptional {

		// ------------------------------------------------

		private _StringOptional;

		// ------------------------------------------------

		static default(): StringOptional;

		static copy(it: StringOptional): StringOptional;

		// ------------------------------------------------

		static Value: null | string;

		static value(it: typeof StringOptional.Value): StringOptional;

		get value(): typeof StringOptional.Value;

		set value(it: typeof StringOptional.Value);

		// ------------------------------------------------

	}

	/** 字符串列表 */
	class StringList {

		// ------------------------------------------------

		private _StringList;

		// ------------------------------------------------

		static default(): StringList;

		static copy(it: StringList): StringList;

		// ------------------------------------------------

		static Value: Array<string>;

		static value(it: typeof StringList.Value): StringList;

		get value(): typeof StringList.Value;

		set value(it: typeof StringList.Value);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 文件路径 */
	class Path {

		// ------------------------------------------------

		private _Path;

		// ------------------------------------------------

		static default(): Path;

		static copy(it: Path): Path;

		// ------------------------------------------------

		static Value: string;

		static value(it: typeof Path.Value): Path;

		get value(): typeof Path.Value;

		set value(it: typeof Path.Value);

		// ------------------------------------------------

	}

	/** Optional包装的Path */
	class PathOptional {

		// ------------------------------------------------

		private _PathOptional;

		// ------------------------------------------------

		static default(): PathOptional;

		static copy(it: PathOptional): PathOptional;

		// ------------------------------------------------

		static Value: null | string;

		static value(it: typeof PathOptional.Value): PathOptional;

		get value(): typeof PathOptional.Value;

		set value(it: typeof PathOptional.Value);

		// ------------------------------------------------

	}

	/** 文件路径列表 */
	class PathList {

		// ------------------------------------------------

		private _PathList;

		// ------------------------------------------------

		static default(): PathList;

		static copy(it: PathList): PathList;

		// ------------------------------------------------

		static Value: Array<string>;

		static value(it: typeof PathList.Value): PathList;

		get value(): typeof PathList.Value;

		set value(it: typeof PathList.Value);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 字节，8位无符号整数 */
	class Byte {

		// ------------------------------------------------

		private _Byte;

		// ------------------------------------------------

		static default(): Byte;

		static copy(it: Byte): Byte;

		// ------------------------------------------------

		static Value: bigint;

		static value(it: typeof Byte.Value): Byte;

		get value(): typeof Byte.Value;

		set value(it: typeof Byte.Value);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 字节序列容器 */
	class ByteArray {

		// ------------------------------------------------

		private _ByteArray;

		// ------------------------------------------------

		static default(): ByteArray;

		static copy(it: ByteArray): ByteArray;

		// ------------------------------------------------

		static Value: ArrayBuffer;

		static value(it: typeof ByteArray.Value): ByteArray;

		get value(): typeof ByteArray.Value;

		set value(it: typeof ByteArray.Value);

		// ------------------------------------------------

		/**
		 * 根据所给尺寸构造数组
		 * @param size 数组尺寸
		 * @returns 对象
		 */
		static allocate(
			size: Size,
		): ByteArray;

		// ------------------------------------------------

		/**
		 * 分配新数组
		 * @param size 新数组的尺寸
		 * @returns 无
		 */
		allocate(
			size: Size,
		): Void;

		/**
		 * 重置数组
		 * @returns 无
		 */
		reset(
		): Void;

		// ------------------------------------------------

		/**
		 * 获取序列尺寸
		 * @returns 序列尺寸
		 */
		size(
		): Size;

		// ------------------------------------------------

		/**
		 * 获取观测本序列的视图
		 * @returns 观测本序列的视图
		 */
		view(
		): ByteListView;

		// ------------------------------------------------

		/**
		 * 移交内存的所有权给所返回的ArrayBuffer对象，调用之后对象表现为重置之后的状态
		 * @returns 持有所有权的ArrayBuffer
		 */
		release(
		): typeof ByteArray.Value;

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 字节序列视图 */
	class ByteListView {

		// ------------------------------------------------

		private _ByteListView;

		// ------------------------------------------------

		static default(): ByteListView;

		static copy(it: ByteListView): ByteListView;

		// ------------------------------------------------

		static Value: ArrayBuffer;

		static value(it: typeof ByteListView.Value): ByteListView;

		get value(): typeof ByteListView.Value;

		set value(it: typeof ByteListView.Value);

		// ------------------------------------------------

		/**
		 * 获取视图尺寸
		 * @returns 视图尺寸
		 */
		size(
		): Size;

		// ------------------------------------------------

		/**
		 * 获取子视图
		 * @param begin 起始位置
		 * @param size 尺寸
		 * @returns 子视图
		 */
		sub(
			begin: Size,
			size: Size,
		): ByteListView;

		// ------------------------------------------------

	}

	/** 变量字节序列视图 */
	type VByteListView = ByteListView;

	/** 常量字节序列视图 */
	type CByteListView = ByteListView;

	// ------------------------------------------------

	/** 字节流视图 */
	class ByteStreamView {

		// ------------------------------------------------

		private _ByteStreamView;

		// ------------------------------------------------

		static default(): ByteStreamView;

		static copy(it: ByteStreamView): ByteStreamView;

		// ------------------------------------------------

		/**
		 * 使用字节序列视图作为观测对象，初始流位置为0
		 * @param list 观测对象
		 * @returns 对象
		 */
		static watch(
			list: ByteListView,
		): ByteStreamView;

		// ------------------------------------------------

		/**
		 * 获取视图尺寸
		 * @returns 视图尺寸
		 */
		size(
		): Size;

		// ------------------------------------------------

		/**
		 * 获取流所处的位置，即已流经的字节数
		 * @returns 流所处的位置
		 */
		position(
		): Size;

		/**
		 * 设置流所处的位置
		 * @param position 位置，不大于视图的尺寸
		 * @returns 无
		 */
		set_position(
			position: Size,
		): Void;

		// ------------------------------------------------

		/**
		 * 获取观测完整序列的视图，即 [ 0, size() )
		 * @returns 观测完整序列的视图
		 */
		view(
		): ByteListView;

		/**
		 * 获取观测已流经序列的视图，即 [ 0, position() )
		 * @returns 观测已流经序列的视图
		 */
		stream_view(
		): ByteListView;

		// ------------------------------------------------

	}

	/** 字节输入流视图 */
	type IByteStreamView = ByteStreamView;

	/** 字节输出流视图 */
	type OByteStreamView = ByteStreamView;

	// ------------------------------------------------

	/** 字符序列视图 */
	class CharacterListView {

		// ------------------------------------------------

		private _CharacterListView;

		// ------------------------------------------------

		static default(): CharacterListView;

		static copy(it: CharacterListView): CharacterListView;

		// ------------------------------------------------

		/**
		 * 获取视图尺寸
		 * @returns 视图尺寸
		 */
		size(
		): Size;

		// ------------------------------------------------

		/**
		 * 获取子视图
		 * @param begin 起始位置
		 * @param size 尺寸
		 * @returns 子视图
		 */
		sub(
			begin: Size,
			size: Size,
		): CharacterListView;

		// ------------------------------------------------

	}

	/** 变量字符序列视图 */
	type VCharacterListView = CharacterListView;

	/** 常量字符序列视图 */
	type CCharacterListView = CharacterListView;

	// ------------------------------------------------

	/** 字符流视图 */
	class CharacterStreamView {

		// ------------------------------------------------

		private _CharacterStreamView;

		// ------------------------------------------------

		static default(): CharacterStreamView;

		static copy(it: CharacterStreamView): CharacterStreamView;

		// ------------------------------------------------

		/**
		 * 使用字符序列视图作为观测对象，初始流位置为0
		 * @param list 观测对象
		 * @returns 对象
		 */
		static watch(
			list: CharacterListView,
		): CharacterStreamView;

		// ------------------------------------------------

		/**
		 * 获取视图尺寸
		 * @returns 视图尺寸
		 */
		size(
		): Size;

		// ------------------------------------------------

		/**
		 * 获取流所处的位置，即已流经的字符数
		 * @returns 流所处的位置
		 */
		position(
		): Size;

		/**
		 * 设置流所处的位置
		 * @param position 位置，不大于视图的尺寸
		 * @returns 无
		 */
		set_position(
			position: Size,
		): Void;

		// ------------------------------------------------

		/**
		 * 获取观测完整序列的视图，即 [ 0, size() )
		 * @returns 观测完整序列的视图
		 */
		view(
		): CharacterListView;

		/**
		 * 获取观测已流经序列的视图，即 [ 0, position() )
		 * @returns 观测已流经序列的视图
		 */
		stream_view(
		): CharacterListView;

		// ------------------------------------------------

	}

	/** 字符输入流视图 */
	type ICharacterStreamView = CharacterStreamView;

	/** 字符输出流视图 */
	type OCharacterStreamView = CharacterStreamView;

	// ------------------------------------------------

	/** JSON */
	namespace JSON {

		/** 值 */
		type JS_Value = undefined | null | boolean | bigint | number | string | JS_Value[] | { [key: string]: JS_Value; };

		/**
		 * 值，存储 null、boolean、bigint、number、string、array、object 类型值的变体
		 * 
		 * Constraint 是为在 JS_Value 基础上对值的进一步约束，仅用于TS的静态类型检查
		 */
		class Value<Constraint extends JS_Value = JS_Value> {

			// ------------------------------------------------

			private _JSON_Value: Constraint;

			// ------------------------------------------------

			static default<Constraint extends JS_Value>(): Value<Constraint>;

			static copy<Constraint extends JS_Value>(it: Value<Constraint>): Value<Constraint>;

			// ------------------------------------------------

			static value<Constraint extends JS_Value>(it: Constraint): Value<Constraint>;

			get value(): Constraint;

			set value(it: Constraint);

			// ------------------------------------------------

		}

	}

	/** XML */
	namespace XML {

		/** 结点类型 */
		type JS_Type = 'element' | 'text' | 'comment';

		/** 元素 */
		type JS_Element = {
			/** 名称 */
			name: string,
			/** 属性 */
			attribute: Record<string, string>,
			/** 子结点 */
			child: Array<JS_Node>,
		};

		/** 文本 */
		type JS_Text = {
			/** 值 */
			value: string,
			/** 是否为cdata */
			cdata: boolean,
		};

		/** 注释 */
		type JS_Comment = {
			/** 值 */
			value: string,
		};

		/** 元素结点 */
		type JS_ElementNode = {
			type: 'element',
			value: JS_Element,
		};

		/** 注释结点 */
		type JS_TextNode = {
			type: 'text',
			value: JS_Text,
		};

		/** 注释结点 */
		type JS_CommentNode = {
			type: 'comment',
			value: JS_Comment,
		};

		/** 结点 */
		type JS_Node = JS_ElementNode | JS_TextNode | JS_CommentNode;

		/** 结点 */
		class Node {

			// ------------------------------------------------

			private _XML_Node;

			// ------------------------------------------------

			static default(): Node;

			static copy(it: Node): Node;

			// ------------------------------------------------

			static value(it: JS_Node): Node;

			get value(): JS_Node;

			set value(it: JS_Node);

			// ------------------------------------------------

		}

	}

	/** 图像 */
	namespace Image {

		// ------------------------------------------------

		/** 表征图像尺寸的整数组 */
		class ImageSize {

			// ------------------------------------------------

			private _Image_ImageSize;

			// ------------------------------------------------

			static default(): ImageSize;

			static copy(it: ImageSize): ImageSize;

			// ------------------------------------------------

			static Value: [bigint, bigint];

			static value(it: typeof ImageSize.Value): ImageSize;

			get value(): typeof ImageSize.Value;

			set value(it: typeof ImageSize.Value);

			// ------------------------------------------------

		}

		/** 表征图像位置的整数组 */
		class ImagePosition {

			// ------------------------------------------------

			private _Image_ImagePosition;

			// ------------------------------------------------

			static default(): ImagePosition;

			static copy(it: ImagePosition): ImagePosition;

			// ------------------------------------------------

			static Value: [bigint, bigint];

			static value(it: typeof ImagePosition.Value): ImagePosition;

			get value(): typeof ImagePosition.Value;

			set value(it: typeof ImagePosition.Value);

			// ------------------------------------------------

		}

		// ------------------------------------------------

		/** 颜色 */
		class Color {

			// ------------------------------------------------

			private _Image_Color;

			// ------------------------------------------------

			static default(): Color;

			static copy(it: Color): Color;

			// ------------------------------------------------

			static Value: bigint;

			static value(it: typeof Color.Value): Color;

			get value(): typeof Color.Value;

			set value(it: typeof Color.Value);

			// ------------------------------------------------

		}

		// ------------------------------------------------

		/** 颜色列表 */
		class ColorList {

			// ------------------------------------------------

			private _Image_ColorList;

			// ------------------------------------------------

			static default(): ColorList;

			static copy(it: ColorList): ColorList;

			// ------------------------------------------------

			static Value: Array<bigint>;

			static value(it: typeof ColorList.Value): ColorList;

			get value(): typeof ColorList.Value;

			set value(it: typeof ColorList.Value);

			// ------------------------------------------------

		}

		// ------------------------------------------------

		/** 像素 */
		class Pixel {

			// ------------------------------------------------

			private _Image_Pixel;

			// ------------------------------------------------

			static default(): Pixel;

			static copy(it: Pixel): Pixel;

			// ------------------------------------------------

			static Value: [bigint, bigint, bigint, bigint];

			static value(it: typeof Pixel.Value): Pixel;

			get value(): typeof Pixel.Value;

			set value(it: typeof Pixel.Value);

			// ------------------------------------------------

		}

		// ------------------------------------------------

		/** 图像 */
		class Image {

			// ------------------------------------------------

			private _Image_Image;

			// ------------------------------------------------

			static default(): Image;

			static copy(it: Image): Image;

			// ------------------------------------------------

			/**
			 * 根据所给尺寸构造图像
			 * @param size 图像尺寸
			 * @returns 对象
			 */
			static allocate(
				size: ImageSize,
			): Image;

			// ------------------------------------------------

			/**
			 * 分配新图像
			 * @param size 新图像的尺寸
			 * @returns 无
			 */
			allocate(
				size: ImageSize,
			): Void;

			/**
			 * 重置图像
			 * @returns 无
			 */
			reset(
			): Void;

			// ------------------------------------------------

			/**
			 * 获取图像尺寸
			 * @returns 图像尺寸
			 */
			size(
			): ImageSize;

			// ------------------------------------------------

			/**
			 * 获取观测本图像的视图
			 * @returns 观测本图像的视图
			 */
			view(
			): ImageView;

			// ------------------------------------------------

		}

		/** 图像视图 */
		class ImageView {

			// ------------------------------------------------

			private _Image_ImageView;

			// ------------------------------------------------

			static default(): ImageView;

			static copy(it: ImageView): ImageView;

			// ------------------------------------------------

			/**
			 * 获取视图尺寸
			 * @returns 视图尺寸
			 */
			size(
			): ImageSize;

			// ------------------------------------------------

			/**
			 * 获取子视图
			 * @param position 起始位置
			 * @param size 尺寸
			 * @returns 子视图
			 */
			sub(
				position: ImagePosition,
				size: ImageSize,
			): ImageView;

			// ------------------------------------------------

			/**
			 * 以像素填充视图
			 * @param pixel 像素
			 * @returns 无
			 */
			fill(
				pixel: Pixel,
			): Void;

			/**
			 * 以图像填充视图
			 * @param image 图像
			 * @returns 无
			 */
			draw(
				image: ImageView,
			): Void;

			// ------------------------------------------------

		}

		/** 变量图像视图 */
		type VImageView = ImageView;

		/** 常量图像视图 */
		type CImageView = ImageView;

		// ------------------------------------------------

	}

	/** 存储 */
	namespace Storage {

		// ------------------------------------------------

		/**
		 * 判断目标是否存在
		 * @param target 目标
		 * @returns 目标存在与否
		 */
		function exist(
			target: Path,
		): Boolean;

		/**
		 * 判断目标是否存在且为文件
		 * @param target 目标
		 * @returns 文件存在与否
		 */
		function exist_file(
			target: Path,
		): Boolean;

		/**
		 * 判断目标是否存在且为目录
		 * @param target 目标
		 * @returns 目录存在与否
		 */
		function exist_directory(
			target: Path,
		): Boolean;

		// ------------------------------------------------

		/**
		 * 复制文件或目录
		 * @param source 来源
		 * @param destination 目的
		 * @returns 无
		 */
		function copy(
			source: Path,
			destination: Path,
		): Void;

		/**
		 * 移动文件或目录
		 * @param source 来源
		 * @param destination 目的
		 * @returns 无
		 */
		function rename(
			source: Path,
			destination: Path,
		): Void;

		/**
		 * 删除文件或目录
		 * @param source 来源
		 * @returns 无
		 */
		function remove(
			source: Path,
		): Void;

		// ------------------------------------------------

		/**
		 * 创建符号链接
		 * @param target 目标
		 * @param object 对象，即链接指向的路径，可以不存在或为非法
		 * @param is_directory 指向的路径是否为目录
		 * @returns 无
		 */
		function create_link(
			target: Path,
			object: Path,
			is_directory: Boolean,
		): Void;

		/**
		 * 解析符号链接
		 * @param target 目标
		 * @returns 链接所指向的对象
		 */
		function parse_link(
			target: Path,
		): Path;

		// ------------------------------------------------

		/**
		 * 创建硬链接
		 * @param target 目标
		 * @param object 对象，即链接所指向的文件系统对象，必须存在
		 * @returns 无
		 */
		function create_hard_link(
			target: Path,
			object: Path,
		): Void;

		// ------------------------------------------------

		/**
		 * 创建文件
		 * @param target 目标文件
		 * @returns 无
		 */
		function create_file(
			target: Path,
		): Void;

		/**
		 * 获取文件尺寸
		 * @param target 目标文件
		 * @returns 文件尺寸
		 */
		function size_file(
			target: Path,
		): Size;

		/**
		 * 调整文件尺寸
		 * @param target 目标文件
		 * @param size 文件尺寸
		 * @returns 无
		 */
		function resize_file(
			target: Path,
			size: Size,
		): Void;

		/**
		 * 读取文件至一个新的字节序列容器
		 * @param target 目标文件
		 * @returns 文件内容
		 */
		function read_file(
			target: Path,
		): ByteArray;

		/**
		 * 将字节序列写入文件
		 * @param target 目标文件
		 * @param data 文件内容
		 * @returns 无
		 */
		function write_file(
			target: Path,
			data: CByteListView,
		): Void;

		/**
		 * 读取文件至字节输出流
		 * @param target 目标文件
		 * @param data 文件内容
		 * @returns 无
		 */
		function read_stream_file(
			target: Path,
			data: OByteStreamView,
		): Void;

		/**
		 * 将字节输入流写入文件
		 * @param target 目标文件
		 * @param data 文件内容
		 * @returns 无
		 */
		function write_stream_file(
			target: Path,
			data: IByteStreamView,
		): Void;

		// ------------------------------------------------

		/**
		 * 创建目录
		 * @param target 目标目录
		 * @returns 无
		 */
		function create_directory(
			target: Path,
		): Void;

		// ------------------------------------------------

		/**
		 * 计数目标目录下的文件与目录
		 * @param target 目标目录
		 * @param depth 需要处理的深度，为空时处理所有层级
		 * @returns 目标目录下的文件与目录数
		 */
		function count(
			target: Path,
			depth: SizeOptional,
		): Size;

		/**
		 * 计数目标目录下的文件
		 * @param target 目标目录
		 * @param depth 需要处理的深度，为空时处理所有层级
		 * @returns 目标目录下的文件数
		 */
		function count_file(
			target: Path,
			depth: SizeOptional,
		): Size;

		/**
		 * 计数目标目录下的目录
		 * @param target 目标目录
		 * @param depth 需要处理的深度，为空时处理所有层级
		 * @returns 目标目录下的目录数
		 */
		function count_directory(
			target: Path,
			depth: SizeOptional,
		): Size;

		/**
		 * 列出目标目录下的文件与目录
		 * @param target 目标目录
		 * @param depth 需要处理的深度，为空时处理所有层级
		 * @returns 目标目录下的文件与目录列表
		 */
		function list(
			target: Path,
			depth: SizeOptional,
		): PathList;

		/**
		 * 列出目标目录下的文件
		 * @param target 目标目录
		 * @param depth 需要处理的深度，为空时处理所有层级
		 * @returns 目标目录下的文件列表
		 */
		function list_file(
			target: Path,
			depth: SizeOptional,
		): PathList;

		/**
		 * 列出目标目录下的目录
		 * @param target 目标目录
		 * @param depth 需要处理的深度，为空时处理所有层级
		 * @returns 目标目录下的目录列表
		 */
		function list_directory(
			target: Path,
			depth: SizeOptional,
		): PathList;

	}

	/** 进程 */
	namespace Process {

		// ------------------------------------------------

		/**
		 * 获取工作目录
		 * @returns 工作目录
		 */
		function get_working_directory(
		): Path;

		/**
		 * 设置工作目录
		 * @param target 工作目录
		 * @returns 无
		 */
		function set_working_directory(
			target: Path,
		): Void;

		// ------------------------------------------------

		/**
		 * 获取环境变量
		 * @param name 变量名
		 * @returns 变量值
		 */
		function get_environment_variable(
			name: String,
		): StringOptional;

		/**
		 * 设置环境变量
		 * @param name 变量名
		 * @param value 变量值
		 * @returns 无
		 */
		function set_environment_variable(
			name: String,
			value: StringOptional,
		): Void;

		/**
		 * 列出环境变量
		 * @returns 环境变量列表
		 */
		function list_environment_variable(
		): StringList;

		// ------------------------------------------------

		/**
		 * 生成子进程
		 * @param program 程序
		 * @param argument 参数
		 * @param environment 环境
		 * @param input 输入
		 * @param output 输出
		 * @param error 错误
		 * @returns 程序正常退出时，返回其退出码
		 */
		function spawn_child(
			program: Path,
			argument: StringList,
			environment: StringList,
			input: PathOptional,
			output: PathOptional,
			error: PathOptional,
		): IntegerU32;

		// ------------------------------------------------

		/**
		 * 调用宿主环境的命令处理器
		 * @param command 命令
		 * @returns std::system的返回值，由实现定义
		 */
		function execute_system_command(
			command: String,
		): IntegerU32;

		// ------------------------------------------------

	}

	/** 工具 */
	namespace Tool {

		/** 数据 */
		namespace Data {

			/** 散列 */
			namespace Hash {

				/** FNV */
				namespace FNV {

					/** 模式 */
					class Mode {

						// ------------------------------------------------

						private _Tool_Data_Hash_FNV_Mode;

						// ------------------------------------------------

						static default(): Mode;

						static copy(it: Mode): Mode;

						// ------------------------------------------------

						static Value: 'm_0' | 'm_1' | 'm_1a';

						static value(it: typeof Mode.Value): Mode;

						get value(): typeof Mode.Value;

						set value(it: typeof Mode.Value);

						// ------------------------------------------------

					}

					/** 位数 */
					class BitCount {

						// ------------------------------------------------

						private _Tool_Data_Hash_FNV_BitCount;

						// ------------------------------------------------

						static default(): BitCount;

						static copy(it: BitCount): BitCount;

						// ------------------------------------------------

						static Value: 'b_32' | 'b_64';

						static value(it: typeof BitCount.Value): BitCount;

						get value(): typeof BitCount.Value;

						set value(it: typeof BitCount.Value);

						// ------------------------------------------------

					}

					/** 散列 */
					namespace Hash {

						/**
						 * 散列
						 * @param data 数据
						 * @param value 散列值
						 * @param mode 模式
						 * @param bit_count 位数
						 */
						function process(
							data: CByteListView,
							value: ByteArray,
							mode: Mode,
							bit_count: BitCount,
						): Void;

					}

				}

				/** MD5 */
				namespace MD5 {

					/** 散列 */
					namespace Hash {

						/**
						 * 散列
						 * @param data 数据
						 * @param value 散列值
						 */
						function process(
							data: CByteListView,
							value: ByteArray,
						): Void;

					}

				}

			}

			/** 编码 */
			namespace Encoding {

				/** Base64 */
				namespace Base64 {

					/** 编码 */
					namespace Encode {

						/**
						 * 编码
						 * @param raw 原始数据
						 * @param ripe 成品数据
						 */
						function process(
							raw: IByteStreamView,
							ripe: OCharacterStreamView,
						): Void;

						/**
						 * 计算成品数据尺寸
						 * @param raw_size 原始数据尺寸
						 * @param ripe_size 成品数据尺寸
						 */
						function estimate(
							raw_size: Size,
							ripe_size: Size,
						): Void;

					}

					/** 解码 */
					namespace Decode {

						/**
						 * 解码
						 * @param ripe 成品数据
						 * @param raw 原始数据
						 */
						function process(
							ripe: ICharacterStreamView,
							raw: OByteStreamView,
						): Void;

						/**
						 * 计算原始数据尺寸
						 * @param ripe 成品数据
						 * @param raw_size 原始数据尺寸
						 */
						function estimate(
							ripe: CCharacterListView,
							raw_size: Size,
						): Void;

					}

				}

			}

			/** 加密 */
			namespace Encryption {

				/** EXOR */
				namespace EXOR {

					/** 加密 */
					namespace Encrypt {

						/**
						 * 加密
						 * @param plain 明文数据
						 * @param cipher 密文数据
						 * @param key 密钥
						 */
						function process(
							plain: IByteStreamView,
							cipher: OByteStreamView,
							key: CByteListView,
						): Void;

					}

				}

				/** Rijndael */
				namespace Rijndael {

					/** 模式 */
					class Mode {

						// ------------------------------------------------

						private _Tool_Data_Encrypt_Rijndeal_Mode;

						// ------------------------------------------------

						static default(): Mode;

						static copy(it: Mode): Mode;

						// ------------------------------------------------

						static Value: 'ecb' | 'cbc' | 'cfb';

						static value(it: typeof Mode.Value): Mode;

						get value(): typeof Mode.Value;

						set value(it: typeof Mode.Value);

						// ------------------------------------------------

					}

					/** 加密 */
					namespace Encrypt {

						/**
						 * 加密
						 * @param plain 明文数据
						 * @param cipher 密文数据
						 * @param mode 模式
						 * @param block_size 块尺寸，允许16、24、32
						 * @param key_size 密钥尺寸，允许16、24、32
						 * @param key 密钥，尺寸必须与key_size一致
						 * @param iv 初始向量，当模式为cbc或cfb时有效，此时尺寸必须与block_size一致
						 */
						function process(
							plain: IByteStreamView,
							cipher: OByteStreamView,
							mode: Mode,
							block_size: Size,
							key_size: Size,
							key: String,
							iv: String,
						): Void;

					}

					/** 解密 */
					namespace Decrypt {

						/**
						 * 解密
						 * @param cipher 密文数据
						 * @param plain 明文数据
						 * @param mode 模式
						 * @param block_size 块尺寸，允许16、24、32
						 * @param key_size 密钥尺寸，允许16、24、32
						 * @param key 密钥，尺寸必须与key_size一致
						 * @param iv 初始向量，当模式为cbc或cfb时有效，此时尺寸必须与block_size一致
						 */
						function process(
							cipher: IByteStreamView,
							plain: OByteStreamView,
							mode: Mode,
							block_size: Size,
							key_size: Size,
							key: String,
							iv: String,
						): Void;

					}

				}

			}

			/** 压缩 */
			namespace Compression {

				/** Deflate */
				namespace Deflate {

					/** 策略 */
					class Strategy {

						// ------------------------------------------------

						private _Tool_Data_Compress_Deflate_Strategy;

						// ------------------------------------------------

						static default(): Strategy;

						static copy(it: Strategy): Strategy;

						// ------------------------------------------------

						static Value: 'default_mode' | 'filtered' | 'huffman_only' | 'rle' | 'fixed';

						static value(it: typeof Strategy.Value): Strategy;

						get value(): typeof Strategy.Value;

						set value(it: typeof Strategy.Value);

						// ------------------------------------------------

					}

					/** 封装 */
					class Wrapper {

						// ------------------------------------------------

						private _Tool_Data_Compress_Deflate_Wrapper;

						// ------------------------------------------------

						static default(): Wrapper;

						static copy(it: Wrapper): Wrapper;

						// ------------------------------------------------

						static Value: 'none' | 'zlib' | 'gzip';

						static value(it: typeof Wrapper.Value): Wrapper;

						get value(): typeof Wrapper.Value;

						set value(it: typeof Wrapper.Value);

						// ------------------------------------------------

					}

					/** 压缩 */
					namespace Compress {

						/**
						 * 压缩
						 * @param raw 原始数据
						 * @param ripe 成品数据
						 * @param level 压缩级别(0~9)
						 * @param window_bits 窗口尺寸(8~15)
						 * @param memory_level 内存级别(1~9)
						 * @param strategy 策略
						 * @param wrapper 封装
						 */
						function process(
							raw: IByteStreamView,
							ripe: OByteStreamView,
							level: Size,
							window_bits: Size,
							memory_level: Size,
							strategy: Strategy,
							wrapper: Wrapper,
						): Void;

						/**
						 * 计算成品数据尺寸上限
						 * @param raw_size 原始数据尺寸
						 * @param ripe_size_bound 成品数据尺寸上限
						 * @param window_bits 窗口尺寸(8~15)
						 * @param memory_level 内存级别(1~9)
						 * @param wrapper 封装
						 */
						function estimate(
							raw_size: Size,
							ripe_size_bound: Size,
							window_bits: Size,
							memory_level: Size,
							wrapper: Wrapper,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param ripe 成品数据
						 * @param raw 原始数据
						 * @param window_bits 窗口尺寸(8~15)
						 * @param wrapper 封装
						 */
						function process(
							ripe: IByteStreamView,
							raw: OByteStreamView,
							window_bits: Size,
							wrapper: Wrapper,
						): Void;

					}

				}

				/** BZip2 */
				namespace BZip2 {

					/** 压缩 */
					namespace Compress {

						/**
						 * 压缩
						 * @param raw 原始数据
						 * @param ripe 成品数据
						 * @param block_size 块尺寸(1~9)
						 * @param work_factor 工作因子(0~250，0==30)
						 */
						function process(
							raw: IByteStreamView,
							ripe: OByteStreamView,
							block_size: Size,
							work_factor: Size,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param ripe 成品数据
						 * @param raw 原始数据
						 * @param small 是否使用少量内存
						 */
						function process(
							ripe: IByteStreamView,
							raw: OByteStreamView,
							small: Boolean,
						): Void;

					}

				}

				/** Lzma */
				namespace Lzma {

					/** 压缩 */
					namespace Compress {

						/**
						 * 压缩
						 * @param raw 原始数据
						 * @param ripe 成品数据
						 * @param level 压缩级别(0~9)
						 */
						function process(
							raw: IByteStreamView,
							ripe: OByteStreamView,
							level: Size,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param ripe 成品数据
						 * @param raw 原始数据
						 */
						function process(
							ripe: IByteStreamView,
							raw: OByteStreamView,
						): Void;

					}

				}

			}

			/** 差异 */
			namespace Differentiation {

				/** VCDiff */
				namespace VCDiff {

					/** 编码 */
					namespace Encode {

						/**
						 * 编码
						 * @param before 变动前数据
						 * @param after 变动后数据
						 * @param patch 补丁数据
						 * @param interleaved 使用交叉模式生成差异数据
						 */
						function process(
							before: IByteStreamView,
							after: IByteStreamView,
							patch: OByteStreamView,
							interleaved: Boolean,
						): Void;

					}

					/** 解码 */
					namespace Decode {

						/**
						 * 解码
						 * @param before 变动前数据
						 * @param after 变动后数据
						 * @param patch 补丁数据
						 * @param maximum_window_size 最大窗口尺寸
						 */
						function process(
							before: IByteStreamView,
							after: OByteStreamView,
							patch: IByteStreamView,
							maximum_window_size: Size,
						): Void;

					}

				}

			}

			/** 序列化 */
			namespace Serialization {

				/** JSON */
				namespace JSON {

					/** 写 */
					namespace Write {

						/**
						 * 写
						 * @param data 数据
						 * @param value 值
						 * @param disable_array_trailing_comma 禁用数组尾随逗号
						 * @param disable_array_line_breaking 禁用数组换行
						 * @param disable_object_trailing_comma 禁用对象尾随逗号
						 * @param disable_object_line_breaking 禁用对象换行
						 */
						function process(
							data: OCharacterStreamView,
							value: Kernel.JSON.Value,
							disable_array_trailing_comma: Boolean,
							disable_array_line_breaking: Boolean,
							disable_object_trailing_comma: Boolean,
							disable_object_line_breaking: Boolean,
						): Void;

					}

					/** 读 */
					namespace Read {

						/**
						 * 读
						 * @param data 数据
						 * @param value 值
						 * @param buffer 缓冲区
						 */
						function process(
							data: ICharacterStreamView,
							value: Kernel.JSON.Value,
							buffer: OCharacterStreamView,
						): Void;

					}

				}

				/** XML */
				namespace XML {

					/** 写 */
					namespace Write {

						/**
						 * 写
						 * @param data 数据
						 * @param value 值
						 */
						function process(
							data: String,
							value: Kernel.XML.Node,
						): Void;

					}

					/** 读 */
					namespace Read {

						/**
						 * 读
						 * @param data 数据
						 * @param value 值
						 */
						function process(
							data: String,
							value: Kernel.XML.Node,
						): Void;

					}

				}

			}

		}

		/** 纹理 */
		namespace Texture {

			/** 变换 */
			namespace Transformation {

				/** 翻转 */
				namespace Flip {

					/**
					 * 翻转
					 * @param target 目标
					 * @param horizontal 水平翻转
					 * @param vertical 垂直翻转
					 */
					function process(
						target: Image.VImageView,
						horizontal: Boolean,
						vertical: Boolean,
					): Void;

				}

				/** 缩放 */
				namespace Scale {

					/**
					 * 缩放
					 * @param source 来源
					 * @param destination 目的
					 */
					function process(
						source: Image.CImageView,
						destination: Image.VImageView,
					): Void;

				}

			}

			/** 编码 */
			namespace Encoding {

				/** 格式 */
				class Format {

					// ------------------------------------------------

					private _Tool_Texture_Encoding_Format;

					// ------------------------------------------------

					static default(): Format;

					static copy(it: Format): Format;

					// ------------------------------------------------

					static Value: 'a_8' | 'rgb_332' | 'rgb_565' | 'rgba_5551' | 'rgba_4444' | 'rgba_8888' | 'argb_1555' | 'argb_4444' | 'argb_8888' | 'l_8' | 'la_44' | 'la_88' | 'al_44' | 'al_88' | 'rgb_888_o' | 'rgba_8888_o';

					static value(it: typeof Format.Value): Format;

					get value(): typeof Format.Value;

					set value(it: typeof Format.Value);

					// ------------------------------------------------

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param image 图像
					 * @param format 格式
					 */
					function process(
						data: OByteStreamView,
						image: Image.CImageView,
						format: Format,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param image 图像
					 * @param format 格式
					 */
					function process(
						data: IByteStreamView,
						image: Image.VImageView,
						format: Format,
					): Void;

				}

			}

			/** 压缩 */
			namespace Compression {

				/** ETC */
				namespace ETC {

					/** 格式 */
					class Format {

						// ------------------------------------------------

						private _Tool_Texture_Compression_ETC_Format;

						// ------------------------------------------------

						static default(): Format;

						static copy(it: Format): Format;

						// ------------------------------------------------

						static Value: 'v1_rgb' | 'v2_rgb' | 'v2_rgba';

						static value(it: typeof Format.Value): Format;

						get value(): typeof Format.Value;

						set value(it: typeof Format.Value);

						// ------------------------------------------------

					}

					/** 压缩 */
					namespace Compress {

						/**
						 * 压缩
						 * @param data 数据
						 * @param image 图像
						 * @param format 格式
						 */
						function process(
							data: OByteStreamView,
							image: Image.CImageView,
							format: Format,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param data 数据
						 * @param image 图像
						 * @param format 格式
						 */
						function process(
							data: IByteStreamView,
							image: Image.VImageView,
							format: Format,
						): Void;

					}

				}

				/** PVRTC */
				namespace PVRTC {

					/** 格式 */
					class Format {

						// ------------------------------------------------

						private _Tool_Texture_Compression_PVRTC_Format;

						// ------------------------------------------------

						static default(): Format;

						static copy(it: Format): Format;

						// ------------------------------------------------

						static Value: 'v1_4bpp_rgb' | 'v1_4bpp_rgba';

						static value(it: typeof Format.Value): Format;

						get value(): typeof Format.Value;

						set value(it: typeof Format.Value);

						// ------------------------------------------------

					}

					/** 压缩 */
					namespace Compress {

						/**
						 * 压缩
						 * @param data 数据
						 * @param image 图像
						 * @param format 格式
						 */
						function process(
							data: OByteStreamView,
							image: Image.CImageView,
							format: Format,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param data 数据
						 * @param image 图像
						 * @param format 格式
						 */
						function process(
							data: IByteStreamView,
							image: Image.VImageView,
							format: Format,
						): Void;

					}

				}

			}

			/** 文件 */
			namespace File {

				/** PNG */
				namespace PNG {

					/** 写 */
					namespace Write {

						/**
						 * 写
						 * @param data 数据
						 * @param image 图像
						 */
						function process(
							data: OByteStreamView,
							image: Image.CImageView,
						): Void;

					}

					/** 读 */
					namespace Read {

						/**
						 * 读
						 * @param data 数据
						 * @param image 图像
						 */
						function process(
							data: IByteStreamView,
							image: Image.VImageView,
						): Void;

						/**
						 * 计算图像尺寸
						 * @param data 数据
						 * @param image_size 图像尺寸
						 */
						function estimate(
							data: CByteListView,
							image_size: Image.ImageSize,
						): Void;

					}

				}

			}

		}

		/** Wwise */
		namespace Wwise {

			/** SoundBank */
			namespace SoundBank {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_Wwise_SoundBank_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 72n | 88n | 112n | 113n | 118n | 120n | 125n | 128n | 132n | 134n | 135n | 140n | 145n | 150n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

						/** 声音库 */
						type SoundBank = {
							/** ID */
							id: bigint;
							/** BNK头扩展数据 */
							header_expand: string;
						};

					}

					/** 声音库 */
					class SoundBank {

						// ------------------------------------------------

						private _Tool_Wwise_SoundBank_Definition_SoundBank;

						// ------------------------------------------------

						static default(): SoundBank;

						static copy(it: SoundBank): SoundBank;

						// ------------------------------------------------

						static json(it: JSON.Value<JS_N.SoundBank>, version: Version): SoundBank;

						get_json(version: Version): JSON.Value<JS_N.SoundBank>;

						set_json(it: JSON.Value<JS_N.SoundBank>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param embedded_media_directory 内嵌媒体目录
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.SoundBank,
						embedded_media_directory: Path,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param embedded_media_directory 内嵌媒体目录
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.SoundBank,
						embedded_media_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

		}

		/** Marmalade */
		namespace Marmalade {

			/** DZip */
			namespace DZip {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_Marmalade_DZip_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 0n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

						/** 数据块 */
						type Chunk = {
							/** 标志 */
							flag: string;
						};

						/** 资源 */
						type Resource = {
							/** 路径 */
							path: string;
							/** 数据块 */
							chunk: Array<Chunk>;
						};

						/** 包 */
						type Package = {
							/** 资源 */
							resource: Array<Resource>;
						};

					}

					/** 包 */
					class Package {

						// ------------------------------------------------

						private _Tool_Marmalade_DZip_Definition_Package;

						// ------------------------------------------------

						static default(): Package;

						static copy(it: Package): Package;

						// ------------------------------------------------

						static json(it: JSON.Value<JS_N.Package>, version: Version): Package;

						get_json(version: Version): JSON.Value<JS_N.Package>;

						set_json(it: JSON.Value<JS_N.Package>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 数据
					 * @param definition 定义
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Package,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param data 数据
					 * @param definition 定义
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Package,
						resource_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

		}

		/** PopCap */
		namespace PopCap {

			/** ZLib */
			namespace ZLib {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_ZLib_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						variant_64: false | true;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 压缩 */
				namespace Compress {

					/**
					 * 压缩
					 * @param raw 原始数据
					 * @param ripe 成品数据
					 * @param level 压缩级别(0~9)
					 * @param window_bits 窗口尺寸(8~15)
					 * @param memory_level 内存级别(1~9)
					 * @param strategy 策略
					 * @param version 版本
					 */
					function process(
						raw: IByteStreamView,
						ripe: OByteStreamView,
						level: Size,
						window_bits: Size,
						memory_level: Size,
						strategy: Data.Compression.Deflate.Strategy,
						version: Version,
					): Void;

					/**
					 * 计算成品数据尺寸上限
					 * @param raw_size 原始数据尺寸
					 * @param ripe_size_bound 成品数据尺寸上限
					 * @param window_bits 窗口尺寸(8~15)
					 * @param memory_level 内存级别(1~9)
					 * @param version 版本
					 */
					function estimate(
						raw_size: Size,
						ripe_size_bound: Size,
						window_bits: Size,
						memory_level: Size,
						version: Version,
					): Void;

				}

				/** 解压 */
				namespace Uncompress {

					/**
					 * 解压
					 * @param ripe 成品数据
					 * @param raw 原始数据
					 * @param window_bits 窗口尺寸(8~15)
					 * @param version 版本
					 */
					function process(
						ripe: IByteStreamView,
						raw: OByteStreamView,
						window_bits: Size,
						version: Version,
					): Void;

					/**
					 * 计算原始数据尺寸
					 * @param ripe 成品数据
					 * @param raw_size 原始数据尺寸
					 * @param version 版本
					 */
					function estimate(
						ripe: CByteListView,
						raw_size: Size,
						version: Version,
					): Void;

				}

			}

			/** CryptData */
			namespace CryptData {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_CryptData_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 加密 */
				namespace Encrypt {

					/**
					 * 加密
					 * @param plain 明文数据
					 * @param cipher 密文数据
					 * @param limit 限度
					 * @param key 密钥
					 * @param version 版本
					 */
					function process(
						plain: IByteStreamView,
						cipher: OByteStreamView,
						limit: Size,
						key: String,
						version: Version,
					): Void;

					/**
					 * 计算密文数据尺寸
					 * @param plain_size 明文数据尺寸
					 * @param cipher_size 密文数据尺寸
					 * @param limit 限度
					 * @param version 版本
					 */
					function estimate(
						plain_size: Size,
						cipher_size: Size,
						limit: Size,
						version: Version,
					): Void;

				}

				/** 解密 */
				namespace Decrypt {

					/**
					 * 解密
					 * @param cipher 密文数据
					 * @param plain 明文数据
					 * @param limit 限度
					 * @param key 密钥
					 * @param version 版本
					 */
					function process(
						cipher: IByteStreamView,
						plain: OByteStreamView,
						limit: Size,
						key: String,
						version: Version,
					): Void;

					/**
					 * 计算明文数据尺寸
					 * @param cipher 密文数据
					 * @param plain_size 明文数据尺寸
					 * @param limit 限度
					 * @param version 版本
					 */
					function estimate(
						cipher: CByteListView,
						plain_size: Size,
						limit: Size,
						version: Version,
					): Void;

				}

			}

			/** ReflectionObjectNotation */
			namespace ReflectionObjectNotation {

				/** ReflectionObjectNotation所能存储的JSON值类型 */
				type JS_ValidValue = boolean | number | bigint | string | JS_ValidValue[] | { [key: string]: JS_ValidValue; };

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_ReflectionObjectNotation_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 1n;
						native_string_encoding_use_utf8: false | true;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param enable_string_index 启用字符串索引，若是，确保同一字符串只会明文编码一次，之后只使用索引值，可减少编码出的数据的尺寸
					 * @param enable_rtid 启用rtid，若是，符合RTID格式的字符串将编码为0x83系列的值单元
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: JSON.Value<JS_ValidValue>,
						enable_string_index: Boolean,
						enable_rtid: Boolean,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: JSON.Value<JS_ValidValue>,
						version: Version,
					): Void;

				}

			}

			/** UTexture */
			namespace UTexture {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_UTexture_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						compress_texture_data: false | true;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param image 图像
					 * @param format 格式
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						image: Image.CImageView,
						format: Texture.Encoding.Format,
						version: Version,
					): Void;

					/**
					 * 计算数据尺寸上限
					 * @param data_size_bound 数据尺寸上限
					 * @param image_size 图像尺寸
					 * @param format 格式
					 * @param version 版本
					 */
					function estimate(
						data_size_bound: Size,
						image_size: Image.ImageSize,
						format: Texture.Encoding.Format,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param image 图像
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						image: Image.VImageView,
						version: Version,
					): Void;

					/**
					 * 计算图像尺寸
					 * @param data 数据
					 * @param image_size 图像尺寸
					 * @param version 版本
					 */
					function estimate(
						data: CByteListView,
						image_size: Image.ImageSize,
						version: Version,
					): Void;

				}

			}

			/** SexyTexture */
			namespace SexyTexture {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_SexyTexture_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 0n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param image 图像
					 * @param format 格式
					 * @param compress_texture_data 压缩纹理数据
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						image: Image.CImageView,
						format: Texture.Encoding.Format,
						compress_texture_data: Boolean,
						version: Version,
					): Void;

					/**
					 * 计算数据尺寸上限
					 * @param data_size_bound 数据尺寸上限
					 * @param image_size 图像尺寸
					 * @param format 格式
					 * @param compress_texture_data 压缩纹理数据
					 * @param version 版本
					 */
					function estimate(
						data_size_bound: Size,
						image_size: Image.ImageSize,
						format: Texture.Encoding.Format,
						compress_texture_data: Boolean,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param image 图像
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						image: Image.VImageView,
						version: Version,
					): Void;

					/**
					 * 计算图像尺寸
					 * @param data 数据
					 * @param image_size 图像尺寸
					 * @param version 版本
					 */
					function estimate(
						data: CByteListView,
						image_size: Image.ImageSize,
						version: Version,
					): Void;

				}

			}

			/** Animation */
			namespace Animation {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_Animation_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 1n | 2n | 3n | 4n | 5n | 6n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

						/** 平移，2x3变换矩阵的xy部分 */
						type TranslateTransform = [
							/** 水平偏移 */
							number,
							/** 垂直偏移 */
							number,
						];

						/** 旋转，可转换为一个矩阵 */
						type RotateTransform = [
							/** 角度 */
							number,
						];

						/** 矩阵，2x3变换矩阵除去xy的部分 */
						type MatrixTransform = [
							/** a */
							number,
							/** b */
							number,
							/** c */
							number,
							/** d */
							number,
						];

						/** 旋转与平移 */
						type RotateTranslateTransform = [...RotateTransform, ...TranslateTransform];

						/** 矩阵与平移 */
						type MatrixTranslateTransform = [...MatrixTransform, ...TranslateTransform];

						/** 变换变体 */
						type VariantTransform = TranslateTransform | RotateTranslateTransform | MatrixTranslateTransform;

						/** 颜色 */
						type Color = [
							/** 红色 */
							number,
							/** 绿色 */
							number,
							/** 蓝色 */
							number,
							/** 不透明度 */
							number,
						];

						/** 矩形 */
						type Rectangle = {
							/** 位置 */
							position: [number, number];
							/** 尺寸 */
							size: [number, number];
						};

						/** 命令 */
						type Command = [
							/** 命令 */
							string,
							/** 参数 */
							string,
						];

						/** 层移除 */
						type LayerRemove = {
							/** 索引 */
							index: bigint;
						};

						/** 层添加 */
						type LayerAppend = {
							/** 索引 */
							index: bigint;
							/** 名称 */
							name: null | string;
							/** 资源索引 */
							resource: bigint;
							/** 是否为精灵 */
							sprite: boolean;
							/** 是否为附加 */
							additive: boolean;
							/** 预加载帧数 */
							preload_frame: bigint;
							/** 时间比率 */
							time_scale: number;
						};

						/** 层变换 */
						type LayerChange = {
							/** 索引 */
							index: bigint;
							/** 变换 */
							transform: VariantTransform;
							/** 颜色 */
							color: null | Color;
							/** 精灵帧编号 */
							sprite_frame_number: null | bigint;
							/** 源矩形 */
							source_rectangle: null | Rectangle;
						};

						/** 帧 */
						type Frame = {
							/** 标签 */
							label: null | string;
							/** 停止 */
							stop: boolean;
							/** 命令 */
							command: Array<Command>;
							/** 层移除 */
							remove: Array<LayerRemove>;
							/** 层添加 */
							append: Array<LayerAppend>;
							/** 层变换 */
							change: Array<LayerChange>;
						};

						/** 精灵 */
						type Sprite = {
							/** 名称。版本4开始存在 */
							name?: string;
							/** 帧率。版本4开始存在 */
							frame_rate?: number;
							/** 工作区间。版本5开始存在 */
							work_area?: [bigint, bigint];
							/** 帧 */
							frame: Array<Frame>;
						};

						/** 图像 */
						type Image = {
							/** 名称 */
							name: string;
							/** 尺寸。版本4开始存在 */
							size?: [bigint, bigint];
							/** 变换。版本1中为RotateTranslateTransform，版本2开始为MatrixTranslateTransform */
							transform: RotateTranslateTransform | MatrixTranslateTransform;
						};

						/** 动画 */
						type Animation = {
							/** 帧率 */
							frame_rate: bigint;
							/** 位置 */
							position: [number, number];
							/** 尺寸 */
							size: [number, number];
							/** 图像 */
							image: Array<Image>;
							/** 精灵 */
							sprite: Array<Sprite>;
							/** 主精灵。版本4开始为可选项 */
							main_sprite: null | Sprite;
						};

					}

					/** 动画 */
					class Animation {

						// ------------------------------------------------

						private _Tool_PopCap_Animation_Definition_Animation;

						// ------------------------------------------------

						static default(): Animation;

						static copy(it: Animation): Animation;

						// ------------------------------------------------

						static json(it: JSON.Value<JS_N.Animation>, version: Version): Animation;

						get_json(version: Version): JSON.Value<JS_N.Animation>;

						set_json(it: JSON.Value<JS_N.Animation>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Animation,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Animation,
						version: Version,
					): Void;

				}

			}

			/** ReAnimation */
			namespace ReAnimation {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_ReAnimation_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						platform: 'desktop' | 'mobile' | 'television';
						variant_64: false | true;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

						/** 精灵 */
						type Transform = {
						};

						/** 图像 */
						type Track = {
							/** 名称 */
							name: string;
							/** 尺寸。版本4开始存在 */
							t: Array<Transform>;
						};

						/** 动画 */
						type Animation = {
							/** 帧率 */
							fps: number;
							/** 位置 */
							track: Array<Track>;
						};

					}

					/** 动画 */
					class Animation {

						// ------------------------------------------------

						private _Tool_PopCap_ReAnimation_Definition_Animation;

						// ------------------------------------------------

						static default(): Animation;

						static copy(it: Animation): Animation;

						// ------------------------------------------------

						static json(it: JSON.Value<undefined>, version: Version): Animation;

						get_json(version: Version): JSON.Value<undefined>;

						set_json(it: JSON.Value<undefined>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Animation,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Animation,
						version: Version,
					): Void;

				}

			}

			/** Particle */
			namespace Particle {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_Particle_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						platform: 'desktop' | 'mobile' | 'television';
						variant_64: false | true;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

					}

					/** 粒子 */
					class Particle {

						// ------------------------------------------------

						private _Tool_PopCap_Particle_Definition_Particle;

						// ------------------------------------------------

						static default(): Particle;

						static copy(it: Particle): Particle;

						// ------------------------------------------------

						static json(it: JSON.Value<undefined>, version: Version): Particle;

						get_json(version: Version): JSON.Value<undefined>;

						set_json(it: JSON.Value<undefined>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Particle,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Particle,
						version: Version,
					): Void;

				}

			}

			/** Trail */
			namespace Trail {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_Trail_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						platform: 'desktop' | 'mobile' | 'television';
						variant_64: false | true;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

					}

					/** 拖尾 */
					class Trail {

						// ------------------------------------------------

						private _Tool_PopCap_Trail_Definition_Trail;

						// ------------------------------------------------

						static default(): Trail;

						static copy(it: Trail): Trail;

						// ------------------------------------------------

						static json(it: JSON.Value<undefined>, version: Version): Trail;

						get_json(version: Version): JSON.Value<undefined>;

						set_json(it: JSON.Value<undefined>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Trail,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Trail,
						version: Version,
					): Void;

				}

			}

			/** RenderEffect */
			namespace RenderEffect {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_RenderEffect_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 1n;
						variant: 1n | 2n | 3n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

					}

					/** 效果 */
					class Effect {

						// ------------------------------------------------

						private _Tool_PopCap_RenderEffect_Definition_Effect;

						// ------------------------------------------------

						static default(): Effect;

						static copy(it: Effect): Effect;

						// ------------------------------------------------

						static json(it: JSON.Value<undefined>, version: Version): Effect;

						get_json(version: Version): JSON.Value<undefined>;

						set_json(it: JSON.Value<undefined>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Effect,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Effect,
						version: Version,
					): Void;

				}

			}

			/** ParticleEffect */
			namespace ParticleEffect {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_ParticleEffect_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 1n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

					}

					/** 效果 */
					class Effect {

						// ------------------------------------------------

						private _Tool_PopCap_ParticleEffect_Definition_Effect;

						// ------------------------------------------------

						static default(): Effect;

						static copy(it: Effect): Effect;

						// ------------------------------------------------

						static json(it: JSON.Value<undefined>, version: Version): Effect;

						get_json(version: Version): JSON.Value<undefined>;

						set_json(it: JSON.Value<undefined>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Effect,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Effect,
						version: Version,
					): Void;

				}

			}

			/** CharacterFontWidget2 */
			namespace CharacterFontWidget2 {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_CharacterFontWidget2_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

					}

					/** 字体部件 */
					class FontWidget {

						// ------------------------------------------------

						private _Tool_PopCap_CharacterFontWidget2_Definition_FontWidget;

						// ------------------------------------------------

						static default(): FontWidget;

						static copy(it: FontWidget): FontWidget;

						// ------------------------------------------------

						static json(it: JSON.Value<undefined>, version: Version): FontWidget;

						get_json(version: Version): JSON.Value<undefined>;

						set_json(it: JSON.Value<undefined>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.FontWidget,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param definition 定义
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.FontWidget,
						version: Version,
					): Void;

				}

			}

			/** Package */
			namespace Package {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_Package_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 0n;
						compress_resource_data: false | true;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

						/** 资源 */
						type Resource = {
							/** 路径 */
							path: string;
							/** 时间 */
							time: bigint;
						};

						/** 包 */
						type Package = {
							/** 资源 */
							resource: Array<Resource>;
						};

					}

					/** 包 */
					class Package {

						// ------------------------------------------------

						private _Tool_PopCap_Definition_Package;

						// ------------------------------------------------

						static default(): Package;

						static copy(it: Package): Package;

						// ------------------------------------------------

						static json(it: JSON.Value<JS_N.Package>, version: Version): Package;

						get_json(version: Version): JSON.Value<JS_N.Package>;

						set_json(it: JSON.Value<JS_N.Package>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 数据
					 * @param definition 定义
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Package,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param data 数据
					 * @param definition 定义
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Package,
						resource_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

			/** ResourceStreamGroup */
			namespace ResourceStreamGroup {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_ResourceStreamGroup_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 1n | 3n | 4n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

						/**
						 * 资源类型
						 * + general 常规资源
						 * + texture 纹理资源
						 */
						type ResourceType = 'general' | 'texture';

						/** 常规资源附加 */
						type GeneralResourceAdditional = {
						};

						/** 纹理资源附加 */
						type TextureResourceAdditional = {
							/** 索引 */
							index: bigint;
							/** 尺寸 */
							size: [bigint, bigint];
						};

						/** 资源附加 */
						type ResourceAdditional = {
							type: 'general';
							value: GeneralResourceAdditional;
						} | {
							type: 'texture';
							value: TextureResourceAdditional;
						};

						/** 资源 */
						type Resource = {
							/** 路径 */
							path: string;
							/** 附加 */
							additional: ResourceAdditional;
						};

						/** 子包压缩 */
						type PacketCompression = {
							/** 以zlib压缩常规资源数据段 */
							general: boolean;
							/** 以zlib压缩纹理资源数据段 */
							texture: boolean;
						};

						/** 包 */
						type Package = {
							/** 压缩 */
							compression: PacketCompression;
							/** 资源 */
							resource: Array<Resource>;
						};

					}

					/** 包 */
					class Package {

						// ------------------------------------------------

						private _Tool_PopCap_ResourceStreamGroup_Definition_Package;

						// ------------------------------------------------

						static default(): Package;

						static copy(it: Package): Package;

						// ------------------------------------------------

						static json(it: JSON.Value<JS_N.Package>, version: Version): Package;

						get_json(version: Version): JSON.Value<JS_N.Package>;

						set_json(it: JSON.Value<JS_N.Package>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 数据
					 * @param definition 定义
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Package,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param data 数据
					 * @param definition 定义
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Package,
						resource_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

			/** ResourceStreamBundle */
			namespace ResourceStreamBundle {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_ResourceStreamBundle_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 1n | 3n | 4n;
						extended_texture_information_for_pvz2_cn: 0n | 1n | 2n | 3n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 定义 */
				namespace Definition {

					namespace JS_N {

						/**
						 * 资源类型
						 * + general 常规资源
						 * + texture 纹理资源
						 */
						type ResourceType = 'general' | 'texture';

						/** 常规资源附加 */
						type GeneralResourceAdditional = {
						};

						/** 纹理资源附加 */
						type TextureResourceAdditional = {
							/** 尺寸 */
							size: [bigint, bigint];
							/** 格式 */
							format: bigint;
							/** 行字节数 */
							pitch: bigint;
							/** 附加字节数。当 version.extended_texture_information_for_pvz2_cn >= 1 时存在 */
							additional_byte_count?: bigint;
							/** 缩放。当 version.extended_texture_information_for_pvz2_cn >= 2 时存在 */
							scale?: bigint;
						};

						/** 资源附加 */
						type ResourceAdditional = {
							type: 'general';
							value: GeneralResourceAdditional;
						} | {
							type: 'texture';
							value: TextureResourceAdditional;
						};

						/** 资源 */
						type Resource = {
							/** 路径 */
							path: string;
							/** 附加 */
							additional: ResourceAdditional;
						};

						/** 子包压缩 */
						type PacketCompression = {
							/** 以zlib压缩常规资源数据段 */
							general: boolean;
							/** 以zlib压缩纹理资源数据段 */
							texture: boolean;
						};

						/** 子群类别 */
						type SubgroupCategory = {
							/** 分辨率 */
							resolution: null | bigint;
							/** 区域。当 version.number >= 3 时存在 */
							locale?: null | string;
						};

						/** 子群 */
						type Subgroup = {
							/** ID */
							id: string;
							/** 类别 */
							category: SubgroupCategory;
							/** 压缩 */
							compression: PacketCompression;
							/** 资源 */
							resource: Array<Resource>;
						};

						/** 群 */
						type Group = {
							/** ID */
							id: string;
							/** 是否为复合群 */
							composite: boolean;
							/** 子群 */
							subgroup: Array<Subgroup>;
						};

						/** 包 */
						type Package = {
							/** 群 */
							group: Array<Group>;
						};

					}

					/** 包 */
					class Package {

						// ------------------------------------------------

						private _Tool_PopCap_ResourceStreamBundle_Definition_Package;

						// ------------------------------------------------

						static default(): Package;

						static copy(it: Package): Package;

						// ------------------------------------------------

						static json(it: JSON.Value<JS_N.Package>, version: Version): Package;

						get_json(version: Version): JSON.Value<JS_N.Package>;

						set_json(it: JSON.Value<JS_N.Package>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 详单 */
				namespace Manifest {

					namespace JS_N {
					}

					/** 包 */
					class PackageOptional {

						// ------------------------------------------------

						private _Tool_PopCap_ResourceStreamBundle_Manifest_PackageOptional;

						// ------------------------------------------------

						static default(): PackageOptional;

						static copy(it: PackageOptional): PackageOptional;

						// ------------------------------------------------

						static json(it: JSON.Value<null>, version: Version): PackageOptional;

						get_json(version: Version): JSON.Value<null>;

						set_json(it: JSON.Value<null>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 数据
					 * @param definition 定义
					 * @param manifest 清单
					 * @param resource_directory 资源目录
					 * @param packet_file 子包文件
					 * @param new_packet_file 新生成子包文件
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						definition: Definition.Package,
						manifest: Manifest.PackageOptional,
						resource_directory: Path,
						packet_file: PathOptional,
						new_packet_file: PathOptional,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param data 数据
					 * @param definition 定义
					 * @param manifest 清单
					 * @param resource_directory 资源目录
					 * @param packet_file 子包文件
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						definition: Definition.Package,
						manifest: Manifest.PackageOptional,
						resource_directory: PathOptional,
						packet_file: PathOptional,
						version: Version,
					): Void;

				}

			}

			/** ResourceStreamBundlePatch */
			namespace ResourceStreamBundlePatch {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_ResourceStreamBundlePatch_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 1n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param before 变动前数据
					 * @param after 变动后数据
					 * @param patch 补丁数据
					 * @param use_raw_packet 使用原始子包
					 * @param version 版本
					 */
					function process(
						before: IByteStreamView,
						after: IByteStreamView,
						patch: OByteStreamView,
						use_raw_packet: Boolean,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param before 变动前数据
					 * @param after 变动后数据
					 * @param patch 补丁数据
					 * @param use_raw_packet 使用原始子包
					 * @param version 版本
					 */
					function process(
						before: IByteStreamView,
						after: OByteStreamView,
						patch: IByteStreamView,
						use_raw_packet: Boolean,
						version: Version,
					): Void;

				}

			}

		}

		/** 杂项 */
		namespace Miscellaneous {

			/** Xbox中的Tiled纹理 */
			namespace XboxTiledTexture {

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param image 图像
					 * @param format 格式
					 */
					function process(
						data: OByteStreamView,
						image: Image.CImageView,
						format: Texture.Encoding.Format,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param image 图像
					 * @param format 格式
					 */
					function process(
						data: IByteStreamView,
						image: Image.VImageView,
						format: Texture.Encoding.Format,
					): Void;

				}

			}

			/** PvZ-2中国版中的alpha调色板纹理 */
			namespace PvZ2CNAlphaPaletteTexture {

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param image 图像
					 * @param palette 调色板
					 */
					function process(
						data: OByteStreamView,
						image: Image.CImageView,
						palette: Image.ColorList,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param image 图像
					 * @param palette 调色板
					 */
					function process(
						data: IByteStreamView,
						image: Image.VImageView,
						palette: Image.ColorList,
					): Void;

				}

			}

			/** PvZ-2中国版中的加密数据 */
			namespace PvZ2CNCryptData {

				/** 加密 */
				namespace Encrypt {

					/**
					 * 加密
					 * @param plain 明文数据
					 * @param cipher 密文数据
					 * @param key 密钥
					 */
					function process(
						plain: IByteStreamView,
						cipher: OByteStreamView,
						key: String,
					): Void;

					/**
					 * 计算密文数据的尺寸
					 * @param plain_size 明文数据尺寸
					 * @param cipher_size 密文数据的尺寸
					 */
					function estimate(
						plain_size: Size,
						cipher_size: Size,
					): Void;

				}

				/** 解密 */
				namespace Decrypt {

					/**
					 * 解密
					 * @param cipher 密文数据
					 * @param plain 明文数据
					 * @param key 密钥
					 */
					function process(
						cipher: IByteStreamView,
						plain: OByteStreamView,
						key: String,
					): Void;

					/**
					 * 计算明文数据的尺寸
					 * @param cipher_size 密文数据尺寸
					 * @param plain_size 明文数据的尺寸
					 */
					function estimate(
						cipher_size: Size,
						plain_size: Size,
					): Void;

				}

			}

		}

	}

	// ------------------------------------------------

	/** 杂项 */
	namespace Miscellaneous {

		// ------------------------------------------------

		/** 线程 */
		class Thread {

			// ------------------------------------------------

			private _Miscellaneous_Thread;

			// ------------------------------------------------

			static default(): Thread;

			// ------------------------------------------------

			/**
			 * 判断线程是否可合并
			 * @returns 线程是否可合并
			 */
			joinable(
			): Boolean;

			// ------------------------------------------------

			/**
			 * 合并线程
			 * @returns 无
			 */
			join(
			): Void;

			/**
			 * 分离线程
			 * @returns 无
			 */
			detach(
			): Void;

			// ------------------------------------------------

			/**
			 * 当前线程放弃目前的执行权，以允许其他线程执行
			 * @returns 无
			 */
			static yield(
			): Void;

			/**
			 * 当前线程休眠
			 * @param duration 休眠时间，单位为毫秒
			 * @returns 无
			 */
			static sleep(
				duration: Size,
			): Void;

			// ------------------------------------------------

		}

		/** 上下文 */
		class Context {

			// ------------------------------------------------

			private _Miscellaneous_Context;

			// ------------------------------------------------

			/**
			 * 执行脚本
			 * @param script 脚本
			 * @param name 名称
			 * @param is_module 是否为模块
			 * @returns 计算值
			 */
			evaluate(
				script: CCharacterListView,
				name: String,
				is_module: Boolean,
			): any;

			/**
			 * 调用回调函数，由外壳程序提供，只应在主线程中调用
			 * @param argument 参数
			 * @returns 结果
			 */
			callback(
				argument: StringList,
			): StringList;

			// ------------------------------------------------

			/**
			 * 产生新的上下文，与当前上下文是兄弟关系
			 * @returns 新的上下文
			 */
			spawn(
			): Context;

			// ------------------------------------------------

			/**
			 * 判断当前上下文是否繁忙，即是否通过execute函数在另一线程中执行着任务
			 * @returns 是否繁忙
			 */
			busy(
			): Boolean;

			/**
			 * 在新线程中执行函数
			 * @param thread 线程
			 * @param executor 函数
			 * @returns 无
			 */
			execute(
				thread: Thread,
				executor: () => any,
			): Void;

			// ------------------------------------------------

			/**
			 * 模块加载时的主目录，默认为null
			 * @returns 引用变量
			 */
			query_module_home(
			): PathOptional;

			/**
			 * 字节流读写数据时是否使用大端序，默认为false，线程作用域
			 * @returns 引用变量
			 */
			query_byte_stream_use_big_endian(
			): Boolean;

			// ------------------------------------------------

		}

		// ------------------------------------------------
		// ByteListView <-> CharacterListView

		function cast_ByteListView_to_CharacterListView(
			t: ByteListView,
		): CharacterListView;

		function cast_CharacterListView_to_ByteListView(
			t: CharacterListView,
		): ByteListView;

		// ------------------------------------------------
		// ByteArray <-> String

		function cast_moveable_ByteArray_to_String(
			t: ByteArray,
		): String;

		function cast_moveable_String_to_ByteArray(
			t: String,
		): ByteArray;

		// ------------------------------------------------
		// String -> CharacterListView

		function cast_String_to_CharacterListView(
			t: String,
		): CharacterListView;

		// ------------------------------------------------
		// CharacterListView -> JS_String

		function cast_CharacterListView_to_JS_String(
			t: CharacterListView,
		): string;

		// ------------------------------------------------

		/** 主上下文 */
		const g_context: Context;

		/** 程序版本 */
		const g_version: String;

		/** 操作系统 */
		const g_system: String;

		/** 处理器架构 */
		const g_architecture: String;

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 主函数 */
	type JS_MainFunction = (
		data: {
			argument: Array<string>;
			result: undefined | Array<string>;
			exception: undefined | any;
		},
	) => void;

	// ------------------------------------------------

}

interface ImportMeta {
	name: string;
}
