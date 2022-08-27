/**
 * JS API of TwinKleS.ToolKit.Core
 * @version 14
 */
declare namespace TwinKleS.Core {

	// ------------------------------------------------

	/** 内部类型定义模板 */
	class T {

		// ------------------------------------------------

		/** 不存在的属性，用于避免TS类型兼容规则对内部类型的影响 */
		private _T: any;

		// ------------------------------------------------

		/* 通用操作 */

		/** 默认构造 */
		static default(): T;

		/** 复制构造 */
		static copy(t: T): T;

		// ------------------------------------------------

		/** JS值操作 */

		/** JS值构造 */
		static value(t: any): T;

		/** JS值getter */
		get value(): any;

		/** JS值setter */
		set value(t: any);

		// ------------------------------------------------

		/** JSON操作 */

		/** JSON构造 */
		static json(t: JSON.Value): T;

		/** JSON getter */
		get json(): JSON.Value;

		/** JSON setter */
		set json(t: JSON.Value);

		// ------------------------------------------------

		/** 其他 */

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** void的同义词 */
	type Void = void;

	// ------------------------------------------------

	type JS_Boolean = boolean;

	/** 布尔值 */
	class Boolean {

		// ------------------------------------------------

		private _Boolean;

		// ------------------------------------------------

		static default(): Boolean;

		static copy(t: Boolean): Boolean;

		// ------------------------------------------------

		static value(t: JS_Boolean): Boolean;

		get value(): JS_Boolean;

		set value(t: JS_Boolean);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	type JS_IntegerU8 = bigint;

	/** 8位无符号整数 */
	class IntegerU8 {

		// ------------------------------------------------

		private _IntegerU8;

		// ------------------------------------------------

		static default(): IntegerU8;

		static copy(t: IntegerU8): IntegerU8;

		// ------------------------------------------------

		static value(t: JS_IntegerU8): IntegerU8;

		get value(): JS_IntegerU8;

		set value(t: JS_IntegerU8);

		// ------------------------------------------------

	}

	type JS_IntegerU32 = bigint;

	/** 32位无符号整数 */
	class IntegerU32 {

		// ------------------------------------------------

		private _IntegerU32;

		// ------------------------------------------------

		static default(): IntegerU32;

		static copy(t: IntegerU32): IntegerU32;

		// ------------------------------------------------

		static value(t: JS_IntegerU32): IntegerU32;

		get value(): JS_IntegerU32;

		set value(t: JS_IntegerU32);

		// ------------------------------------------------

	}

	type JS_IntegerU64 = bigint;

	/** 64位无符号整数 */
	class IntegerU64 {

		// ------------------------------------------------

		private _IntegerU64;

		// ------------------------------------------------

		static default(): IntegerU64;

		static copy(t: IntegerU64): IntegerU64;

		// ------------------------------------------------

		static value(t: JS_IntegerU64): IntegerU64;

		get value(): JS_IntegerU64;

		set value(t: JS_IntegerU64);

		// ------------------------------------------------

	}

	type JS_IntegerS32 = bigint;

	/** 32位有符号整数 */
	class IntegerS32 {

		// ------------------------------------------------

		private _IntegerS;

		// ------------------------------------------------

		static default(): IntegerS32;

		static copy(t: IntegerS32): IntegerS32;

		// ------------------------------------------------

		static value(t: JS_IntegerS32): IntegerS32;

		get value(): JS_IntegerS32;

		set value(t: JS_IntegerS32);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	type JS_Size = bigint;

	/** 表征尺寸的整数，表现为64位无符号整数 */
	class Size {

		// ------------------------------------------------

		private _Size;

		// ------------------------------------------------

		static default(): Size;

		static copy(t: Size): Size;

		// ------------------------------------------------

		static value(t: JS_Size): Size;

		get value(): JS_Size;

		set value(t: JS_Size);

		// ------------------------------------------------

	}

	type JS_SizeOptional = null | JS_Size;

	/** Optional包装的Path */
	class SizeOptional {

		// ------------------------------------------------

		private _SizeOptional;

		// ------------------------------------------------

		static default(): SizeOptional;

		static copy(t: SizeOptional): SizeOptional;

		// ------------------------------------------------

		static value(t: JS_SizeOptional): SizeOptional;

		get value(): JS_SizeOptional;

		set value(t: JS_SizeOptional);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	type JS_Byte = bigint;

	/** 表征字节的整数，表现为8位无符号整数 */
	class Byte {

		// ------------------------------------------------

		private _Byte;

		// ------------------------------------------------

		static default(): Byte;

		static copy(t: Byte): Byte;

		// ------------------------------------------------

		static value(t: JS_Byte): Byte;

		get value(): JS_Byte;

		set value(t: JS_Byte);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	type JS_String = string;

	/** 字符串 */
	class String {

		// ------------------------------------------------

		private _String;

		// ------------------------------------------------

		static default(): String;

		static copy(t: String): String;

		// ------------------------------------------------

		static value(t: JS_String): String;

		get value(): JS_String;

		set value(t: JS_String);

		// ------------------------------------------------

	}

	type JS_StringList = Array<string>;

	/** 字符串列表 */
	class StringList {

		// ------------------------------------------------

		private _StringList;

		// ------------------------------------------------

		static default(): StringList;

		static copy(t: StringList): StringList;

		// ------------------------------------------------

		static value(t: JS_StringList): StringList;

		get value(): JS_StringList;

		set value(t: JS_StringList);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	type JS_Path = string;

	/** 文件路径 */
	class Path {

		// ------------------------------------------------

		private _Path;

		// ------------------------------------------------

		static default(): Path;

		static copy(t: Path): Path;

		// ------------------------------------------------

		static value(t: JS_Path): Path;

		get value(): JS_Path;

		set value(t: JS_Path);

		// ------------------------------------------------

	}

	type JS_PathOptional = null | JS_Path;

	/** Optional包装的Path */
	class PathOptional {

		// ------------------------------------------------

		private _PathOptional;

		// ------------------------------------------------

		static default(): PathOptional;

		static copy(t: PathOptional): PathOptional;

		// ------------------------------------------------

		static value(t: JS_PathOptional): PathOptional;

		get value(): JS_PathOptional;

		set value(t: JS_PathOptional);

		// ------------------------------------------------

	}

	type JS_PathList = Array<string>;

	/** 文件路径列表 */
	class PathList {

		// ------------------------------------------------

		private _PathList;

		// ------------------------------------------------

		static default(): PathList;

		static copy(t: PathList): PathList;

		// ------------------------------------------------

		static value(t: JS_PathList): PathList;

		get value(): JS_PathList;

		set value(t: JS_PathList);

		// ------------------------------------------------

	}

	// ------------------------------------------------

	type JS_ByteArray = ArrayBuffer;

	/** 字节序列容器 */
	class ByteArray {

		// ------------------------------------------------

		private _ByteArray;

		// ------------------------------------------------

		static default(): ByteArray;

		static copy(t: ByteArray): ByteArray;

		// ------------------------------------------------

		static value(t: JS_ByteArray): ByteArray;

		get value(): JS_ByteArray;

		set value(t: JS_ByteArray);

		// ------------------------------------------------

		/**
		 * 根据所给大小构造数组
		 * @param size 数组大小
		 */
		static allocate(
			size: Size,
		): ByteArray;

		// ------------------------------------------------

		/**
		 * 分配新数组
		 * @param size 新数组的大小
		 */
		allocate(
			size: Size,
		): Void;

		/**
		 * 重置数组
		 */
		reset(
		): Void;

		// ------------------------------------------------

		/**
		 * 获取序列大小
		 * @returns 序列大小
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
		 */
		release(
		): JS_ByteArray;

		// ------------------------------------------------

	}

	// ------------------------------------------------

	type JS_ByteListView = ArrayBuffer;

	/** 字节序列视图 */
	class ByteListView {

		// ------------------------------------------------

		private _ByteListView;

		// ------------------------------------------------

		static default(): ByteListView;

		static copy(t: ByteListView): ByteListView;

		// ------------------------------------------------

		static value(t: JS_ByteListView): ByteListView;

		get value(): JS_ByteListView;

		set value(t: JS_ByteListView);

		// ------------------------------------------------

		/**
		 * 获取视图大小
		 * @returns 视图大小
		 */
		size(
		): Size;

		// ------------------------------------------------

		/**
		 * 获取子视图
		 * @param begin 起始位置
		 * @param size 大小
		 * @returns 子视图
		 */
		sub(
			begin: Size,
			size: Size,
		): ByteListView;

		// ------------------------------------------------

	}

	type JS_VByteListView = JS_ByteListView;

	/** 变量字节序列视图 */
	type VByteListView = ByteListView;

	type JS_CByteListView = JS_ByteListView;

	/** 常量字节序列视图 */
	type CByteListView = ByteListView;

	// ------------------------------------------------

	/** 字节流视图 */
	class ByteStreamView {

		// ------------------------------------------------

		private _ByteStreamView;

		// ------------------------------------------------

		static default(): ByteStreamView;

		static copy(t: ByteStreamView): ByteStreamView;

		// ------------------------------------------------

		/**
		 * 使用字节序列视图作为观测对象，初始流位置为0
		 * @param list 观测对象
		 */
		static look(
			list: ByteListView,
		): ByteStreamView;

		// ------------------------------------------------

		/**
		 * 获取视图大小
		 * @returns 视图大小
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
		 * @param position 位置，不大于视图的大小
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

		/**
		 * 从流的当前位置读取一个字节
		 * @returns 读取到的字节
		 */
		read(
		): Byte;

		/**
		 * 向流的当前位置写入一个字节
		 * @param value 字节值
		 */
		write(
			value: Byte,
		): Void;

		// ------------------------------------------------

	}

	/** 字节输入流视图 */
	type IByteStreamView = ByteStreamView;

	/** 字节输出流视图 */
	type OByteStreamView = ByteStreamView;

	/** 字节流读写数据时是否使用大端序，默认为false */
	const g_byte_stream_use_big_endian: Boolean;

	// ------------------------------------------------

	/** 字符序列视图 */
	class CharacterListView {

		// ------------------------------------------------

		private _CharacterListView;

		// ------------------------------------------------

		static default(): CharacterListView;

		static copy(t: CharacterListView): CharacterListView;

		// ------------------------------------------------

		/**
		 * 获取视图大小
		 * @returns 视图大小
		 */
		size(
		): Size;

		// ------------------------------------------------

		/**
		 * 获取子视图
		 * @param begin 起始位置
		 * @param size 大小
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

		static copy(t: CharacterStreamView): CharacterStreamView;

		// ------------------------------------------------

		/**
		 * 使用字符序列视图作为观测对象，初始流位置为0
		 * @param list 观测对象
		 */
		static look(
			list: CharacterListView,
		): CharacterStreamView;

		// ------------------------------------------------

		/**
		 * 获取视图大小
		 * @returns 视图大小
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
		 * @param position 位置，不大于视图的大小
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

		type JS_Value = undefined | null | boolean | bigint | number | string | JS_Value[] | { [key: string]: JS_Value; };

		/**
		 * JSON值，存储 null、boolean、bigint、number、string、array、object 类型值的变体
		 * 
		 * Constraint 是为在 JS_Value 基础上对值的进一步约束，仅用于TS的静态类型检查
		 */
		class Value<Constraint extends JS_Value = JS_Value> {

			// ------------------------------------------------

			private _JSON_Value: Constraint;

			// ------------------------------------------------

			static default<Constraint extends JS_Value>(): Value<Constraint>;

			static copy<Constraint extends JS_Value>(t: Value<Constraint>): Value<Constraint>;

			// ------------------------------------------------

			static value<Constraint extends JS_Value>(t: Constraint): Value<Constraint>;

			get value(): Constraint;

			set value(t: Constraint);

			// ------------------------------------------------

			static json<Constraint extends JS_Value>(t: JSON.Value<Constraint>): Value<Constraint>;

			get json(): JSON.Value<Constraint>;

			set json(t: JSON.Value<Constraint>);

			// ------------------------------------------------

		}

		/** 写 */
		namespace Write {

			/**
			 * 写至字符串
			 * @param value JSON
			 * @param string 字符串
			 * @param disable_trailing_comma 禁用尾随逗号
			 * @param disable_array_wrap_line 禁用数组元素换行
			 * @param indent_level 缩进计数
			 */
			function process<Constraint extends JS_Value>(
				string: OCharacterStreamView,
				value: Value<Constraint>,
				disable_trailing_comma: Boolean,
				disable_array_wrap_line: Boolean,
				indent_level: Size,
			): Void;

		}

		/** 读 */
		namespace Read {

			/**
			 * 读自字符串
			 * @param string 字符串
			 * @param value JSON
			 */
			function process<Constraint extends JS_Value>(
				string: ICharacterStreamView,
				value: Value<Constraint>,
			): Void;

		}

	}

	/** XML */
	namespace XML {

		/** 结点类型 */
		type JS_Type = 'element' | 'text' | 'comment';

		/** 元素 */
		type JS_Element = {
			/** 名称 */
			name: JS_String,
			/** 属性 */
			attribute: Record<JS_String, JS_String>,
			/** 子结点 */
			child: Array<JS_Node>,
		};

		/** 文本 */
		type JS_Text = {
			/** 值 */
			value: JS_String,
			/** 是否为cdata */
			cdata: JS_Boolean,
		};

		/** 注释 */
		type JS_Comment = {
			/** 值 */
			value: JS_String,
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

			static copy(t: Node): Node;

			// ------------------------------------------------

			static value(t: JS_Node): Node;

			get value(): JS_Node;

			set value(t: JS_Node);

			// ------------------------------------------------

		}

		/** 写 */
		namespace Write {

			/**
			 * 写至字符串
			 * @param value XML
			 * @returns 字符串
			 */
			function process(
				value: Node,
			): String;

		}

		/** 读 */
		namespace Read {

			/**
			 * 读自字符串
			 * @param string 字符串
			 * @returns XML
			 */
			function process(
				string: CCharacterListView,
			): Node;

		}

	}

	/** 图像 */
	namespace Image {

		// ------------------------------------------------

		type JS_ImageSize = [JS_Size, JS_Size];

		/** 表征图像尺寸的整数组 */
		class ImageSize {

			// ------------------------------------------------

			private _Image_ImageSize;

			// ------------------------------------------------

			static default(): ImageSize;

			static copy(t: ImageSize): ImageSize;

			// ------------------------------------------------

			static value(t: JS_ImageSize): ImageSize;

			get value(): JS_ImageSize;

			set value(t: JS_ImageSize);

			// ------------------------------------------------

		}

		type JS_ImagePosition = [JS_Size, JS_Size];

		/** 表征图像位置的整数组 */
		class ImagePosition {

			// ------------------------------------------------

			private _Image_ImagePosition;

			// ------------------------------------------------

			static default(): ImagePosition;

			static copy(t: ImagePosition): ImagePosition;

			// ------------------------------------------------

			static value(t: JS_ImagePosition): ImagePosition;

			get value(): JS_ImagePosition;

			set value(t: JS_ImagePosition);

			// ------------------------------------------------

		}

		// ------------------------------------------------

		type JS_Pixel = [JS_IntegerU8, JS_IntegerU8, JS_IntegerU8, JS_IntegerU8];

		/** 像素 */
		class Pixel {

			// ------------------------------------------------

			private _Image_Pixel;

			// ------------------------------------------------

			static default(): Pixel;

			static copy(t: Pixel): Pixel;

			// ------------------------------------------------

			static value(t: JS_Pixel): Pixel;

			get value(): JS_Pixel;

			set value(t: JS_Pixel);

			// ------------------------------------------------

		}

		// ------------------------------------------------

		/** 位图 */
		class Bitmap {

			// ------------------------------------------------

			private _Image_Bitmap;

			// ------------------------------------------------

			static default(): Bitmap;

			static copy(t: Bitmap): Bitmap;

			// ------------------------------------------------

			/**
			 * 根据所给大小构造位图
			 * @param size 位图大小
			 */
			static allocate(
				size: ImageSize,
			): Bitmap;

			// ------------------------------------------------

			/**
			 * 分配新位图
			 * @param size 新位图的大小
			 */
			allocate(
				size: ImageSize,
			): Void;

			/**
			 * 重置位图
			 */
			reset(
			): Void;

			// ------------------------------------------------

			/**
			 * 获取位图大小
			 * @returns 位图大小
			 */
			size(
			): ImageSize;

			// ------------------------------------------------

			/**
			 * 获取观测本位图的视图
			 * @returns 观测本位图的视图
			 */
			view(
			): BitmapView;

			// ------------------------------------------------

		}

		/** 位图视图 */
		class BitmapView {

			// ------------------------------------------------

			private _Image_BitmapView;

			// ------------------------------------------------

			static default(): BitmapView;

			static copy(t: BitmapView): BitmapView;

			// ------------------------------------------------

			/**
			 * 获取视图大小
			 * @returns 视图大小
			 */
			size(
			): ImageSize;

			// ------------------------------------------------

			/**
			 * 获取子视图
			 * @param position 起始位置
			 * @param size 大小
			 * @returns 子视图
			 */
			sub(
				position: ImagePosition,
				size: ImageSize,
			): BitmapView;

			// ------------------------------------------------

			/**
			 * 以像素填充视图
			 * @param pixel 像素
			 */
			fill(
				pixel: Pixel,
			): Void;

			/**
			 * 以图像填充视图
			 * @param image 图像
			 */
			draw(
				image: BitmapView,
			): Void;

			// ------------------------------------------------

		}

		/** 变量位图视图 */
		type VBitmapView = BitmapView;

		/** 常量位图视图 */
		type CBitmapView = BitmapView;

		// ------------------------------------------------

		/** 文件支持 */
		namespace File {

			/** PNG文件 */
			namespace PNG {

				/**
				 * 获取PNG的图像尺寸
				 * @param data 数据
				 * @returns 图像尺寸
				 */
				function size(
					data: CByteListView,
				): ImageSize;

				/**
				 * 读取PNG至图像
				 * @param data 数据
				 * @param image 图像
				 */
				function read(
					data: IByteStreamView,
					image: VBitmapView,
				): Void;

				/**
				 * 将图像写入PNG
				 * @param data 数据
				 * @param image 图像
				 */
				function write(
					data: OByteStreamView,
					image: CBitmapView,
				): Void;

				/**
				 * 读取PNG至一个新的图像
				 * @param data 数据
				 * @returns 图像
				 */
				function read_of(
					data: IByteStreamView,
				): Bitmap;

				/**
				 * 获取PNG文件的图像尺寸
				 * @param path 文件
				 * @returns 图像尺寸
				 */
				function size_file(
					path: Path,
				): ImageSize;

				/**
				 * 读取PNG文件至图像
				 * @param path 文件
				 * @param image 图像
				 */
				function read_file(
					path: Path,
					image: VBitmapView,
				): Void;

				/**
				 * 将图像写入PNG文件
				 * @param path 文件
				 * @param image 图像
				 */
				function write_file(
					path: Path,
					image: CBitmapView,
				): Void;

				/**
				 * 读取PNG文件至一个新的图像
				 * @param path 文件
				 * @returns 图像
				 */
				function read_file_of(
					path: Path,
				): Bitmap;

			}

		}

		// ------------------------------------------------

	}

	/** 文件系统 */
	namespace FileSystem {

		// ------------------------------------------------

		/**
		 * 判断路径是否存在
		 * @param target 路径
		 * @returns 路径存在与否
		 */
		function exist(
			target: Path,
		): Boolean;

		/**
		 * 判断路径是否存在且为文件
		 * @param target 文件路径
		 * @returns 文件存在与否
		 */
		function exist_file(
			target: Path,
		): Boolean;

		/**
		 * 判断路径是否存在且为目录
		 * @param target 目录路径
		 * @returns 目录存在与否
		 */
		function exist_directory(
			target: Path,
		): Boolean;

		// ------------------------------------------------

		/**
		 * 复制文件或目录
		 * @param source 源路径
		 * @param dest 目的路径
		 */
		function copy(
			source: Path,
			dest: Path,
		): Void;

		/**
		 * 移动文件或目录
		 * @param source 源路径
		 * @param dest 目的路径
		 */
		function rename(
			source: Path,
			dest: Path,
		): Void;

		/**
		 * 删除文件或目录
		 * @param source 源路径
		 */
		function remove(
			source: Path,
		): Void;

		// ------------------------------------------------

		/**
		 * 创建符号链接
		 * @param target 目标
		 * @param object 对象，即链接指向的路径，可以不存在或为非法
		 * @param is_directory 指向路径是否为目录
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
		 */
		function create_hard_link(
			target: Path,
			object: Path,
		): Void;

		// ------------------------------------------------

		/**
		 * 获取文件大小
		 * @param target 目标文件
		 * @returns 文件大小
		 */
		function size_file(
			target: Path,
		): Size;

		/**
		 * 调整文件大小
		 * @param target 目标文件
		 * @param size 文件大小
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
		 */
		function write_file(
			target: Path,
			data: CByteListView,
		): Void;

		/**
		 * 读取文件至字节输出流
		 * @param target 目标文件
		 * @param data 文件内容
		 */
		function read_stream_file(
			target: Path,
			data: OByteStreamView,
		): Void;

		/**
		 * 将字节输入流写入文件
		 * @param target 目标文件
		 * @param data 文件内容
		 */
		function write_stream_file(
			target: Path,
			data: IByteStreamView,
		): Void;

		// ------------------------------------------------

		/**
		 * 创建目录
		 * @param target 目标目录
		 */
		function create_directory(
			target: Path,
		): Void;

		// ------------------------------------------------

		/**
		 * 计数目标目录下的文件与目录
		 * @param target 目标目录
		 * @param depth 需要计算的深度，为空时计算所有层级
		 * @returns 目标目录下的文件与目录数
		 */
		function count(
			target: Path,
			depth: SizeOptional,
		): Size;

		/**
		 * 计数目标目录下的文件
		 * @param target 目标目录
		 * @param depth 需要计算的深度，为空时计算所有层级
		 * @returns 目标目录下的文件数
		 */
		function count_file(
			target: Path,
			depth: SizeOptional,
		): Size;

		/**
		 * 计数目标目录下的目录
		 * @param target 目标目录
		 * @param depth 需要计算的深度，为空时计算所有层级
		 * @returns 目标目录下的目录数
		 */
		function count_directory(
			target: Path,
			depth: SizeOptional,
		): Size;

		/**
		 * 获取目标目录下的文件与目录路径
		 * @param target 目标目录
		 * @param depth 需要获取的深度，为空时获取所有层级
		 * @returns 目标目录下的文件与目录路径列表
		 */
		function list(
			target: Path,
			depth: SizeOptional,
		): PathList;

		/**
		 * 获取目标目录下的文件路径
		 * @param target 目标目录
		 * @param depth 需要获取的深度，为空时获取所有层级
		 * @returns 目标目录下的文件路径列表
		 */
		function list_file(
			target: Path,
			depth: SizeOptional,
		): PathList;

		/**
		 * 获取目标目录下的目录路径
		 * @param target 目标目录
		 * @param depth 需要获取的深度，为空时获取所有层级
		 * @returns 目标目录下的目录路径列表
		 */
		function list_directory(
			target: Path,
			depth: SizeOptional,
		): PathList;

		// ------------------------------------------------

		/**
		 * 获取当前工作目录
		 * @returns 当前工作目录
		 */
		function get_working_directory(
		): Path;

		/**
		 * 改变当前工作目录
		 * @param target 当前工作目录
		 */
		function set_working_directory(
			target: Path,
		): Void;

		/**
		 * 获取当前工作目录
		 * @returns 当前工作目录
		 */
		function get_temporary_directory(
		): Path;

	}

	/** 系统 */
	namespace System {

		// ------------------------------------------------

		/**
		 * 退出程序
		 * @param code 退出状态
		 */
		function exit(
			code: IntegerS32,
		): Void;

		/**
		 * 休眠一段时间，释放CPU使用
		 * @param time 休眠时间，单位为毫秒
		 */
		function sleep(
			time: Size,
		): Void;

		// ------------------------------------------------

		/**
		 * 调用宿主环境的命令处理器
		 * @param command 命令参数
		 */
		function system(
			command: String,
		): IntegerS32;

		/**
		 * 执行外部程序
		 * @param path 路径
		 * @param argument 参数
		 */
		function process(
			path: Path,
			argument: StringList,
		): IntegerS32;

		// ------------------------------------------------

	}

	/** 工具 */
	namespace Tool {

		/** 数据 */
		namespace Data {

			/** 哈希 */
			namespace Hash {

				/** FNV */
				namespace FNV {

					/** 哈希 */
					namespace Hash {

						/**
						 * 计算32位FNV-1值
						 * @param data 数据
						 * @returns 32位FNV-1值
						 */
						function process_1_32(
							data: CByteListView,
						): IntegerU32;

						/**
						 * 计算64位FNV-1值
						 * @param data 数据
						 * @returns 64位FNV-1值
						 */
						function process_1_64(
							data: CByteListView,
						): IntegerU64;

						/**
						 * 计算32位FNV-1a值
						 * @param data 数据
						 * @returns 32位FNV-1a值
						 */
						function process_1a_32(
							data: CByteListView,
						): IntegerU32;

						/**
						 * 计算64位FNV-1a值
						 * @param data 数据
						 * @returns 64位FNV-1a值
						 */
						function process_1a_64(
							data: CByteListView,
						): IntegerU64;

					}

				}

				/** MD5 */
				namespace MD5 {

					/** 哈希 */
					namespace Hash {

						/**
						 * 计算MD5值
						 * @param data 数据
						 * @returns MD5值（字节数组表示）
						 */
						function process(
							data: CByteListView,
						): ByteArray;

						/**
						 * 计算MD5值，并将结果转换为字符串
						 * @param data 数据
						 * @param use_upper_case_number 使用大写数字
						 * @returns MD5值（字符串表示）
						 */
						function process_to_string(
							data: CByteListView,
							use_upper_case_number: Boolean,
						): String;

					}

				}

			}

			/** 编码 */
			namespace Encode {

				/** Base64 */
				namespace Base64 {

					/** 编码 */
					namespace Encode {

						/**
						 * 计算编码后数据的大小
						 * @param raw_size 原始数据大小
						 * @returns 编码后数据的大小
						 */
						function compute_size(
							raw_size: Size,
						): Size;

						/**
						 * 编码
						 * @param raw 原始数据
						 * @param ripe 编码后数据
						 */
						function process(
							raw: IByteStreamView,
							ripe: OCharacterStreamView,
						): Void;

					}

					/** 解码 */
					namespace Decode {

						/**
						 * 计算原始数据的大小
						 * @param ripe 编码后数据
						 * @returns 原始数据的大小
						 */
						function compute_size(
							ripe: CCharacterListView,
						): Size;

						/**
						 * 解码
						 * @param ripe 编码后数据
						 * @param raw 原始数据
						 */
						function process(
							ripe: ICharacterStreamView,
							raw: OByteStreamView,
						): Void;

					}

				}

			}

			/** 加密 */
			namespace Encrypt {

				/** 异或 */
				namespace XOR {

					/** 加密 */
					namespace Crypt {

						/**
						 * 加密
						 * @param plain 原始数据
						 * @param cipher 加密后数据
						 * @param key 密钥
						 */
						function process(
							plain: IByteStreamView,
							cipher: OByteStreamView,
							key: Byte,
						): Void;

					}

				}

				/** Rijndael */
				namespace Rijndael {

					type JS_Mode = 'ecb' | 'cbc' | 'cfb';

					/**
					 * 模式枚举
					 * + ecb
					 * + cbc
					 * + cfb
					 */
					class Mode {

						// ------------------------------------------------

						private _Tool_Data_Encrypt_Rijndeal_Mode;

						// ------------------------------------------------

						static default(): Mode;

						static copy(t: Mode): Mode;

						// ------------------------------------------------

						static value(t: JS_Mode): Mode;

						get value(): JS_Mode;

						set value(t: JS_Mode);

						// ------------------------------------------------

					}

					/** 加密 */
					namespace Encrypt {

						/**
						 * 加密
						 * @param plain 明文数据
						 * @param cipher 密文数据
						 * @param mode 模式
						 * @param block_size 块大小，允许16、24、32
						 * @param key_size 密钥大小，允许16、24、32
						 * @param key 密钥，大小必须与key_size一致
						 * @param iv 初始向量，当模式为cbc或cfb时有效，此时大小必须与block_size一致
						 */
						function process(
							plain: IByteStreamView,
							cipher: OByteStreamView,
							mode: Mode,
							block_size: Size,
							key_size: Size,
							key: String,
							iv: String,
						): None;

					}

					/** 解密 */
					namespace Decrypt {

						/**
						 * 解密
						 * @param cipher 密文数据
						 * @param plain 明文数据
						 * @param mode 模式
						 * @param block_size 块大小，允许16、24、32
						 * @param key_size 密钥大小，允许16、24、32
						 * @param key 密钥，大小必须与key_size一致
						 * @param iv 初始向量，当模式为cbc或cfb时有效，此时大小必须与block_size一致
						 */
						function process(
							cipher: IByteStreamView,
							plain: OByteStreamView,
							mode: Mode,
							block_size: Size,
							key_size: Size,
							key: String,
							iv: String,
						): None;

					}

				}

			}

			/** 压缩 */
			namespace Compress {

				/** Deflate */
				namespace Deflate {

					type JS_Strategy = 'default_mode' | 'filtered' | 'huffman_only' | 'rle' | 'fixed';

					/**
					 * 策略枚举
					 * + default_mode
					 * + filtered
					 * + huffman_only
					 * + rle
					 * + fixed
					 */
					class Strategy {

						// ------------------------------------------------

						private _Tool_Data_Compress_Deflate_Strategy;

						// ------------------------------------------------

						static default(): Strategy;

						static copy(t: Strategy): Strategy;

						// ------------------------------------------------

						static value(t: JS_Strategy): Strategy;

						get value(): JS_Strategy;

						set value(t: JS_Strategy);

						// ------------------------------------------------

					}

					type JS_Wrapper = 'none' | 'zlib' | 'gzip';

					/**
					 * 封装枚举
					 * + none
					 * + zlib
					 * + gzip
					 */
					class Wrapper {

						// ------------------------------------------------

						private _Tool_Data_Compress_Deflate_Wrapper;

						// ------------------------------------------------

						static default(): Wrapper;

						static copy(t: Wrapper): Wrapper;

						// ------------------------------------------------

						static value(t: JS_Wrapper): Wrapper;

						get value(): JS_Wrapper;

						set value(t: JS_Wrapper);

						// ------------------------------------------------

					}

					/** 压缩 */
					namespace Compress {

						/**
						 * 计算压缩后数据的大小的最大可能值
						 * @param raw_size 原始数据大小
						 * @param window_bits 窗口大小(8~15)
						 * @param memory_level 内存级别(1~9)
						 * @param wrapper 封装
						 * @returns 压缩后数据的大小的最大可能值
						 */
						function compute_size_bound(
							raw_size: Size,
							window_bits: Size,
							memory_level: Size,
							wrapper: Wrapper,
						): Size;

						/**
						 * 压缩
						 * @param raw 原始数据
						 * @param ripe 压缩后数据
						 * @param level 压缩级别(0~9)
						 * @param window_bits 窗口大小(8~15)
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

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param ripe 压缩后数据
						 * @param raw 原始数据
						 * @param window_bits 窗口大小(8~15)
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
						 * @param ripe 压缩后数据
						 * @param block_size 块大小(1~9)
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
						 * @param ripe 压缩后数据
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
						 * @param ripe 压缩后数据
						 * @param props 压缩后数据的props
						 * @param level 压缩级别(0~9)
						 */
						function process(
							raw: IByteStreamView,
							ripe: OByteStreamView,
							props: OByteStreamView,
							level: Size,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param ripe 压缩后数据
						 * @param raw 原始数据
						 * @param props 压缩后数据的props
						 */
						function process(
							ripe: IByteStreamView,
							raw: OByteStreamView,
							props: IByteStreamView,
						): Void;

					}

				}

			}

		}

		/** 纹理 */
		namespace Texture {

			// ------------------------------------------------

			type JS_Format = 'a_8' | 'rgb_888' | 'rgba_8888' | 'rgb_565_l' | 'rgba_4444_l' | 'rgba_5551_l' | 'argb_4444_l' | 'argb_8888_l';

			/**
			 * 格式枚举
			 * + a_8
			 * + rgb_888
			 * + rgba_8888
			 * + rgb_565_l
			 * + rgba_4444_l
			 * + rgba_5551_l
			 * + argb_4444_l
			 * + argb_8888_l
			 */
			class Format {

				// ------------------------------------------------

				private _Tool_Texture_Format;

				// ------------------------------------------------

				static default(): Format;

				static copy(t: Format): Format;

				// ------------------------------------------------

				static value(t: JS_Format): Format;

				get value(): JS_Format;

				set value(t: JS_Format);

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
					image: CBitmapView,
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
					image: VBitmapView,
					format: Format,
				): Void;

			}

			// ------------------------------------------------

			/** 压缩 */
			namespace Compress {

				/** ETC1 */
				namespace ETC1 {

					/** 压缩 */
					namespace Compress {

						/**
						 * 压缩
						 * @param data 数据
						 * @param image 图像
						 */
						function process(
							data: OByteStreamView,
							image: CBitmapView,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param data 数据
						 * @param image 图像
						 */
						function process(
							data: IByteStreamView,
							image: VBitmapView,
						): Void;

					}

				}

				/** PVRTC4 */
				namespace PVRTC4 {

					/** 压缩 */
					namespace Compress {

						/**
						 * 压缩
						 * @param data 数据
						 * @param image 图像
						 * @param with_alpha 是否将alpha通道压缩进数据
						 */
						function process(
							data: OByteStreamView,
							image: CBitmapView,
							with_alpha: Boolean,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param data 数据
						 * @param image 图像
						 * @param with_alpha 是否将alpha通道解压至图像
						 */
						function process(
							data: IByteStreamView,
							image: VBitmapView,
							with_alpha: Boolean,
						): Void;

					}

				}

			}

			// ------------------------------------------------

		}

		/** Wwise */
		namespace Wwise {

			/** EncodedMedia */
			namespace EncodedMedia {

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param ripe 已编码的音频数据
					 * @param ffmpeg_file ffmpeg可执行文件的路径，pcm、adpcm、aac、vorbis音频解码时需要调用该程序
					 * @param ww2ogg_file ww2ogg可执行文件的路径，vorbis音频解码时需要调用该程序
					 * @param ww2ogg_pcb_file ww2ogg-pcb文件的路径，vorbis音频解码时需要使用该文件
					 * @param temporary_directory 临时文件目录
					 * @returns 解码后的音频数据
					 */
					function process(
						ripe: CByteListView,
						ffmpeg_file: Path,
						ww2ogg_file: Path,
						ww2ogg_pcb_file: Path,
						temporary_directory: Path,
					): ByteArray;

				}

			}

			/** SoundBank */
			namespace SoundBank {

				type JS_Version = {
					number: 112n;
				};

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_Wwise_SoundBank_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(t: Version): Version;

					// ------------------------------------------------

					static json(t: JSON.Value<JS_Version>): Version;

					get json(): JSON.Value<JS_Version>;

					set json(t: JSON.Value<JS_Version>);

					// ------------------------------------------------

				}

				/** 清单 */
				namespace Manifest {

					namespace JS_N {

						/** HIRC */
						type HIRC = {
							/** ID */
							id: bigint,
							/** 类型 */
							type: bigint,
							/** 数据 */
							data: string,
						};

						/** 声音库 */
						type SoundBank = {
							/** ID */
							id: bigint;
							/** 文件头拓展数据 */
							header_expand: string;
							/** STMG */
							stmg: string;
							/** 内嵌的音频 */
							embedded_audio: Array<bigint> | null;
							/** HIRC */
							hirc: Array<HIRC> | null;
							/** 引用的声音库 */
							referenced_sound_bank: Array<string> | null;
							/** ENVS */
							envs: string;
						};

					}

					/** 声音库 */
					class SoundBankVariant {

						// ------------------------------------------------

						private _Tool_Wwise_SoundBank_Manifest_SoundBankVariant;

						// ------------------------------------------------

						static default(): SoundBankVariant;

						static copy(t: SoundBankVariant): SoundBankVariant;

						// ------------------------------------------------

						static from_json(t: JSON.Value<JS_N.SoundBank>, version: Version): SoundBankVariant;

						to_json(version: Version): JSON.Value<JS_N.SoundBank>;

						from_json(t: JSON.Value<JS_N.SoundBank>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 数据
					 * @param manifest 清单
					 * @param embedded_audio_directory 内嵌音频目录
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						manifest: Manifest.SoundBankVariant,
						embedded_audio_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param data 数据
					 * @param manifest 清单
					 * @param embedded_audio_directory 内嵌音频目录
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						manifest: Manifest.SoundBankVariant,
						embedded_audio_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

		}

		/** Marmalade */
		namespace Marmalade {

			/** DZip */
			namespace DZip {

				type JS_Version = {
					number: 0n;
				};

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_Marmalade_DZip_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(t: Version): Version;

					// ------------------------------------------------

					static json(t: JSON.Value<JS_Version>): Version;

					get json(): JSON.Value<JS_Version>;

					set json(t: JSON.Value<JS_Version>);

					// ------------------------------------------------

				}

				/** 清单 */
				namespace Manifest {

					namespace JS_N {

						/** 数据块 */
						type Chunk = {
							/** 标志 */
							flag: string;
						};

						/** 资源 */
						type Resource = {
							/** 存储方式 */
							chunk: Array<Chunk>;
						};

						/** 包 */
						type Package = {
							/** 资源 */
							resource: Record<string, Resource>;
						};

					}

					/** 包 */
					class PackageVariant {

						// ------------------------------------------------

						private _Tool_Marmalade_DZip_Manifest_PackageVariant;

						// ------------------------------------------------

						static default(): PackageVariant;

						static copy(t: PackageVariant): PackageVariant;

						// ------------------------------------------------

						static from_json(t: JSON.Value<JS_N.Package>, version: Version): PackageVariant;

						to_json(version: Version): JSON.Value<JS_N.Package>;

						from_json(t: JSON.Value<JS_N.Package>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 包
					 * @param manifest 清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						manifest: Manifest.Manifest,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param data 包
					 * @param manifest 清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						manifest: Manifest.Manifest,
						resource_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

		}

		/** PopCap */
		namespace PopCap {

			/** ZLib（附有原始数据大小信息的ZLib） */
			namespace ZLib {

				/** 压缩 */
				namespace Compress {

					/**
					 * 计算压缩后数据的大小的最大可能值
					 * @param raw_size 原始数据大小
					 * @param window_bits 窗口大小(8~15)
					 * @param memory_level 内存级别(1~9)
					 * @param variant_64 是否为64位变体
					 * @returns 压缩后数据的大小的最大可能值
					 */
					function compute_size_bound(
						raw_size: Size,
						window_bits: Size,
						memory_level: Size,
						variant_64: Boolean,
					): Size;

					/**
					 * 压缩
					 * @param raw 原始数据
					 * @param ripe 压缩后数据
					 * @param level 压缩级别(0~9)
					 * @param window_bits 窗口大小(8~15)
					 * @param memory_level 内存级别(1~9)
					 * @param strategy 策略
					 * @param variant_64 是否为64位变体
					 */
					function process(
						raw: IByteStreamView,
						ripe: OByteStreamView,
						level: Size,
						window_bits: Size,
						memory_level: Size,
						strategy: Data.Compress.Deflate.Strategy,
						variant_64: Boolean,
					): Void;

				}

				/** 解压 */
				namespace Uncompress {

					/**
					 * 计算原始数据的大小
					 * @param ripe 压缩后数据
					 * @param variant_64 是否为64位变体
					 * @returns 原始数据的大小
					 */
					function compute_size(
						ripe: CByteListView,
						variant_64: Boolean,
					): Size;

					/**
					 * 解压
					 * @param ripe 压缩后数据
					 * @param raw 原始数据
					 * @param window_bits 窗口大小(8~15)
					 * @param variant_64 是否为64位变体
					 */
					function process(
						ripe: IByteStreamView,
						raw: OByteStreamView,
						window_bits: Size,
						variant_64: Boolean,
					): Void;

				}

			}

			/** Reanim */
			namespace Reanim {

				type JS_Version = {
					platform: 'desktop' | 'phone';
					variant_64: boolean;
				};

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_Reanim_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(t: Version): Version;

					// ------------------------------------------------

					static json(t: JSON.Value<JS_Version>): Version;

					get json(): JSON.Value<JS_Version>;

					set json(t: JSON.Value<JS_Version>);

					// ------------------------------------------------

				}

				/** 清单 */
				namespace Manifest {

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
					class AnimationVariant {

						// ------------------------------------------------

						private _Tool_PopCap_PAM_Manifest_AnimationVariant;

						// ------------------------------------------------

						static default(): AnimationVariant;

						static copy(t: AnimationVariant): AnimationVariant;

						// ------------------------------------------------

						static from_json(t: JSON.Value<>, version: Version): AnimationVariant;

						to_json(version: Version): JSON.Value<>;

						from_json(t: JSON.Value<>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param manifest 清单
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						manifest: Manifest.AnimationVariant,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param manifest 清单
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						manifest: Manifest.AnimationVariant,
						version: Version,
					): Void;

				}

			}

			/** RTON */
			namespace RTON {

				type JS_Version = {
					number: 1n;
				};

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_RTON_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(t: Version): Version;

					// ------------------------------------------------

					static json(t: JSON.Value<JS_Version>): Version;

					get json(): JSON.Value<JS_Version>;

					set json(t: JSON.Value<JS_Version>);

					// ------------------------------------------------

				}

				/** RTON所能存储的JSON值类型 */
				type JS_ValidValue = boolean | number | bigint | string | JS_ValidValue[] | { [key: string]: JS_ValidValue; };

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param rton rton
					 * @param json json
					 * @param enable_string_index 启用字符串索引，若是，确保同一字符串只会明文编码一次，之后只使用索引值，可减少编码出的rton的大小
					 * @param enable_rtid 启用rtid，若是，符合RTID格式的字符串将编码为0x83系列的rton值单元
					 * @param version 版本
					 */
					function process(
						rton: OByteStreamView,
						json: JSON.Value<JS_ValidValue>,
						enable_string_index: Boolean,
						enable_rtid: Boolean,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param rton rton
					 * @param json json
					 * @param version 版本
					 */
					function process(
						rton: IByteStreamView,
						json: JSON.Value<JS_ValidValue>,
						version: Version,
					): Void;

				}

				/** 加密 */
				namespace Encrypt {

					/**
					 * 计算密文数据的大小
					 * @param plain_size 明文数据大小
					 * @returns 密文数据的大小
					 */
					function compute_size(
						plain_size: Size,
					): Size;

					/**
					 * 加密rton
					 * @param plain 明文数据
					 * @param cipher 密文数据
					 * @param key 密钥
					 */
					function process(
						plain: IByteStreamView,
						cipher: OByteStreamView,
						key: String,
					): Void;

				}

				/** 解密 */
				namespace Decrypt {

					/**
					 * 计算明文数据的大小
					 * @param cipher_size 密文数据大小
					 * @returns 明文数据的大小
					 */
					function compute_size(
						cipher_size: Size,
					): Size;

					/**
					 * 解密rton
					 * @param cipher 密文数据
					 * @param plain 明文数据
					 * @param key 密钥
					 */
					function process(
						cipher: IByteStreamView,
						plain: OByteStreamView,
						key: String,
					): Void;

				}

			}

			/** PAM */
			namespace PAM {

				type JS_Version = {
					number: 1n | 2n | 3n | 4n | 5n | 6n;
				};

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_PAM_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(t: Version): Version;

					// ------------------------------------------------

					static json(t: JSON.Value<JS_Version>): Version;

					get json(): JSON.Value<JS_Version>;

					set json(t: JSON.Value<JS_Version>);

					// ------------------------------------------------

				}

				/** 清单 */
				namespace Manifest {

					namespace JS_N {

						/** 平移，2x3变换矩阵的xy部分 */
						type TranslateTransform = [number, number];

						/** 旋转，可转换为一个矩阵 */
						type RotateTransform = [number];

						/** 矩阵，2x3变换矩阵除去xy的部分 */
						type MatrixTransform = [number, number, number, number];

						/** 旋转与平移 */
						type RotateTranslateTransform = [...RotateTransform, ...TranslateTransform];

						/** 矩阵与平移 */
						type MatrixTranslateTransform = [...MatrixTransform, ...TranslateTransform];

						/** 变换变体 */
						type VariantTransform = TranslateTransform | RotateTranslateTransform | MatrixTranslateTransform;

						/** 颜色 */
						type Color = [number, number, number, number];

						/** 命令 */
						type Command = {
							/** 命令 */
							command: string;
							/** 参数 */
							argument: string;
						};

						/** 矩形 */
						type Rectangle = {
							/** 位置 */
							position: [number, number];
							/** 大小 */
							size: [number, number];
						};

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
							/** 时率 */
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
							name: undefined | string;
							/** 帧率。版本4开始存在 */
							frame_rate: undefined | number;
							/** 工作区间。版本5开始存在 */
							work_area: undefined | [bigint, bigint];
							/** 帧 */
							frame: Array<Frame>;
						};

						/** 图像 */
						type Image = {
							/** 名称 */
							name: string;
							/** 尺寸。版本4开始存在 */
							size: undefined | [bigint, bigint];
							/** 变换。版本1中为RotateTranslateTransform，之后为MatrixTranslateTransform */
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
							main_sprite: Sprite | null;
						};

					}

					/** 动画 */
					class AnimationVariant {

						// ------------------------------------------------

						private _Tool_PopCap_PAM_Manifest_AnimationVariant;

						// ------------------------------------------------

						static default(): AnimationVariant;

						static copy(t: AnimationVariant): AnimationVariant;

						// ------------------------------------------------

						static from_json(t: JSON.Value<>, version: Version): AnimationVariant;

						to_json(version: Version): JSON.Value<>;

						from_json(t: JSON.Value<>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param manifest 清单
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						manifest: Manifest.AnimationVariant,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param manifest 清单
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						manifest: Manifest.AnimationVariant,
						version: Version,
					): Void;

				}

			}

			/** PAK */
			namespace PAK {

				type JS_Version = {
					number: 0n;
					compress_resource_data: false | true;
				};

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_PAK_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(t: Version): Version;

					// ------------------------------------------------

					static json(t: JSON.Value<JS_Version>): Version;

					get json(): JSON.Value<JS_Version>;

					set json(t: JSON.Value<JS_Version>);

					// ------------------------------------------------

				}

				/** 清单 */
				namespace Manifest {

					namespace JS_N {

						/** 资源 */
						type Resource = {
							/** 时间 */
							time: bigint;
						};

						/** 包 */
						type Package = {
							/** 资源 */
							resource: Record<string, Resource>;
						};

					}

					/** 包 */
					class PackageVariant {

						// ------------------------------------------------

						private _Tool_PopCap_PAK_Manifest_PackageVariant;

						// ------------------------------------------------

						static default(): PackageVariant;

						static copy(t: PackageVariant): PackageVariant;

						// ------------------------------------------------

						static from_json(t: JSON.Value<JS_N.Package>, version: Version): PackageVariant;

						to_json(version: Version): JSON.Value<JS_N.Package>;

						from_json(t: JSON.Value<JS_N.Package>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 数据
					 * @param manifest 清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						manifest: Manifest.PackageVariant,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param data 数据
					 * @param manifest 清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						manifest: Manifest.PackageVariant,
						resource_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

			/** RSGP数据包 */
			namespace RSGP {

				type JS_Version = {
					number: 3n | 4n;
				};

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_RSGP_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(t: Version): Version;

					// ------------------------------------------------

					static json(t: JSON.Value<JS_Version>): Version;

					get json(): JSON.Value<JS_Version>;

					set json(t: JSON.Value<JS_Version>);

					// ------------------------------------------------

				}

				/** 清单 */
				namespace Manifest {

					namespace JS_N {

						/**
						 * 资源类型
						 * + generic 常规资源
						 * + texture 纹理资源
						 */
						type ResourceType = 'generic' | 'texture';

						/** 常规资源附加 */
						type GenericResourceAdditional = {
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
							type: 'generic';
							value: GenericResource;
						} | {
							type: 'texture';
							value: TextureResource;
						};

						/** 资源 */
						type Resource = {
							additional: ResourceAdditional;
						};

						/** 资源数据段存储方式 */
						type ResourceDataSectionStoreMode = [
							/** 以zlib压缩常规资源数据段 */
							boolean,
							/** 以zlib压缩纹理资源数据段 */
							boolean,
						];

						/** 包 */
						type Package = {
							/** 资源 */
							resource: Record<string, Resource>;
							/** 资源数据段存储方式 */
							resource_data_section_store_mode: ResourceDataSectionStoreMode;
						};

					}

					/** 包 */
					class PackageVariant {

						// ------------------------------------------------

						private _Tool_PopCap_RSGP_Manifest_PackageVariant;

						// ------------------------------------------------

						static default(): PackageVariant;

						static copy(t: PackageVariant): PackageVariant;

						// ------------------------------------------------

						static from_json(t: JSON.Value<JS_N.Package>, version: Version): PackageVariant;

						to_json(version: Version): JSON.Value<JS_N.Package>;

						from_json(t: JSON.Value<JS_N.Package>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 数据
					 * @param manifest 清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						manifest: Manifest.PackageVariant,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param data 数据
					 * @param manifest 清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						manifest: Manifest.PackageVariant,
						resource_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

			/** RSB数据包 */
			namespace RSB {

				type JS_Version = {
					number: 3n | 4n;
					additional_texture_information_for_pvz_2_chinese_android: 0n | 1n | 2n;
				};

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_RSB_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(t: Version): Version;

					// ------------------------------------------------

					static json(t: JSON.Value<JS_Version>): Version;

					get json(): JSON.Value<JS_Version>;

					set json(t: JSON.Value<JS_Version>);

					// ------------------------------------------------

				}

				/** 清单 */
				namespace Manifest {

					namespace JS_N {

						/**
						 * 资源类型
						 * + generic 常规资源
						 * + texture 纹理资源
						 */
						type ResourceType = 'generic' | 'texture';

						/** 常规资源附加 */
						type GenericResourceAdditional = {
						};

						/** 纹理资源附加 */
						type TextureResourceAdditional = {
							/** 尺寸 */
							size: [bigint, bigint];
							/** 缩放。当 version.additional_texture_information_for_pvz_2_chinese_android == 2 时存在 */
							scale: undefined | bigint;
							/** 格式 */
							format: bigint;
						};

						/** 资源附加 */
						type ResourceAdditional = {
							type: 'generic';
							value: GenericResource;
						} | {
							type: 'texture';
							value: TextureResource;
						};

						/** 资源 */
						type Resource = {
							additional: ResourceAdditional;
						};

						/** 资源数据段存储方式 */
						type ResourceDataSectionStoreMode = [
							/** 以zlib压缩常规资源数据段 */
							boolean,
							/** 以zlib压缩纹理资源数据段 */
							boolean,
						];

						/** 子群类别 */
						type SubgroupCategory = [null | bigint, null | string];

						/** 子群 */
						type Subgroup = {
							/** 类别 */
							category: SubgroupCategory;
							/** 资源 */
							resource: Record<string, Resource>;
							/** 资源数据段存储方式 */
							resource_data_section_store_mode: ResourceDataSectionStoreMode;
						};

						/** 群 */
						type Group = {
							/** 是否为复合群 */
							composite: boolean;
							/** 子群 */
							subgroup: Record<string, Subgroup>;
						};

						/** 包 */
						type Package = {
							/** 群 */
							group: Record<string, Group>;
						};

					}

					/** 包 */
					class PackageVariant {

						// ------------------------------------------------

						private _Tool_PopCap_RSB_Manifest_PackageVariant;

						// ------------------------------------------------

						static default(): PackageVariant;

						static copy(t: PackageVariant): PackageVariant;

						// ------------------------------------------------

						static from_json(t: JSON.Value<JS_N.Package>, version: Version): PackageVariant;

						to_json(version: Version): JSON.Value<JS_N.Package>;

						from_json(t: JSON.Value<JS_N.Package>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 详单 */
				namespace Description {

					/** 包 */
					class PackageOptionalVariant {

						// ------------------------------------------------

						private _Tool_PopCap_RSB_Description_PackageOptionalVariant;

						// ------------------------------------------------

						static default(): PackageOptionalVariant;

						static copy(t: PackageOptionalVariant): PackageOptionalVariant;

						// ------------------------------------------------

						static from_json(t: JSON.Value<>, version: Version): PackageOptionalVariant;

						to_json(version: Version): JSON.Value<>;

						from_json(t: JSON.Value<>, version: Version): Void;

						// ------------------------------------------------

					}

				}

				/** 打包 */
				namespace Pack {

					/**
					 * 打包
					 * @param data 数据
					 * @param manifest 清单
					 * @param description 详单
					 * @param resource_directory 资源目录
					 * @param packet_file RSGP子包文件
					 * @param new_packet_file 新生成RSGP子包文件
					 * @param version 版本
					 */
					function process(
						data: OByteStreamView,
						manifest: Manifest.PackageVariant,
						description: Description.PackageOptionalVariant,
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
					 * @param manifest 清单
					 * @param description 详单
					 * @param resource_directory 资源目录
					 * @param packet_file RSGP子包文件
					 * @param version 版本
					 */
					function process(
						data: IByteStreamView,
						manifest: Manifest.PackageVariant,
						description: Description.PackageOptionalVariant,
						resource_directory: PathOptional,
						packet_file: PathOptional,
						version: Version,
					): Void;

				}

			}

		}

		/** 杂项 */
		namespace Misc {

			/** PvZ1-iOS版RSB中的2x系列纹理 */
			namespace PvZ1RSBTexture20SeriesLayout {

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
						image: Image.CBitmapView,
						format: Texture.Format,
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
						image: Image.VBitmapView,
						format: Texture.Format,
					): Void;

				}

			}

			/** PvZ2-安卓中文版RSB中的纹理alpha映射 */
			namespace PvZ2CHSRSBTextureAlphaIndex {

				type JS_IndexList = Array<JS_IntegerU8>;

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param image 图像
					 * @param index alpha索引
					 */
					function process(
						data: OByteStreamView,
						image: Image.CBitmapView,
						index: JS_IndexList,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param image 图像
					 * @param index alpha索引
					 */
					function process(
						data: IByteStreamView,
						image: Image.VBitmapView,
						index: JS_IndexList,
					): Void;

				}

			}

		}

	}

	// ------------------------------------------------

	/** 杂项 */
	namespace Misc {

		/** 上下文 */
		class Thread {

			// ------------------------------------------------

			private _Thread;

			// ------------------------------------------------

			static default(): Thread;

			// ------------------------------------------------

		}

		/** 上下文 */
		class Context {

			// ------------------------------------------------

			private _Context;

			// ------------------------------------------------

			/**
			 * 执行脚本
			 * @param script 脚本
			 * @returns 计算值
			 */
			evaluate(
				script: CCharacterListView,
				name: String,
			): any;

			/**
			 * 调用外壳程序提供的回调函数
			 * @param argument 参数
			 * @returns 结果，只能对之取值，不可修改
			 */
			shell_callback(
				argument: StringList,
			): StringList;

			// ------------------------------------------------

			/**
			 * 产生新的子上下文
			 * @returns 子上下文
			 */
			spawn(
			): Context;

			/**
			 * 出让一次CPU时间给其他线程
			 */
			yield(
			): Void;

			/**
			 * 在新线程中执行函数
			 * @param executor 函数
			 * @param thread 线程
			 */
			execute(
				executor: () => any,
				thread: Thread,
			): Void;

			/**
			 * 获取execute的执行状态。调用execute函数后，若对应线程上函数未执行完毕，则繁忙，返回true，否则空闲，返回false
			 * @returns 执行状态
			 */
			state(
			): Boolean;

			/**
			 * 获取execute的执行结果。仅当state返回true时才可调用
			 * @returns 执行结果
			 */
			result(
			): any;

			// ------------------------------------------------

		}

		/** 当前上下文对象 */
		const g_context: Context;

		// ------------------------------------------------
		// ByteListView <-> CharacterListView

		function cast_ByteListView_to_CharacterListView(
			t: ByteListView,
		): CharacterListView;

		function cast_CharacterListView_to_ByteListView(
			t: CharacterListView,
		): ByteListView;

		// ------------------------------------------------
		// String -> CharacterListView

		function cast_String_to_CharacterListView(
			t: String,
		): CharacterListView;

		// ------------------------------------------------
		// String -> ByteArray

		function cast_moveable_String_to_ByteArray(
			t: String,
		): ByteArray;

		// ------------------------------------------------
		// CharacterListView -> JS_String

		function cast_CharacterListView_to_JS_String(
			t: CharacterListView,
		): JS_String;

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 主函数；返回值为null时，表示执行成功，为string时，表示执行失败，返回值为错误信息 */
	type JS_MainFunction = (
		/** 脚本路径；若不以文件形式运行脚本，则为null */
		script_path: null | string,
		/** 启动参数 */
		argument: Array<string>,
	) => null | string;

	// ------------------------------------------------

}
