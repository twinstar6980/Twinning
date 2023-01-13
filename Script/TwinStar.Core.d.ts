/**
 * JS API of TwinStar.ToolKit.Core
 * @version 22
 */
declare namespace TwinStar.Core {

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

		private _IntegerS;

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

		static copy(it: CharacterListView): CharacterListView;

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

		static copy(it: CharacterStreamView): CharacterStreamView;

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

		/** 位图 */
		class Bitmap {

			// ------------------------------------------------

			private _Image_Bitmap;

			// ------------------------------------------------

			static default(): Bitmap;

			static copy(it: Bitmap): Bitmap;

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

			static copy(it: BitmapView): BitmapView;

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
		 * 创建文件
		 * @param target 目标文件
		 */
		function create_file(
			target: Path,
		): Void;

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
		 * 获取临时目录
		 * @returns 临时目录
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
			code: IntegerU32,
		): Void;

		/**
		 * 执行外部程序
		 * @param program 程序
		 * @param argument 参数
		 * @param redirect_input 重定向输入文件
		 * @param redirect_output 重定向输出文件
		 * @param redirect_error 重定向错误文件
		 */
		function execute(
			program: Path,
			argument: StringList,
			redirect_input: PathOptional,
			redirect_output: PathOptional,
			redirect_error: PathOptional,
		): IntegerU32;

		/**
		 * 调用宿主环境的命令处理器
		 * @param command 命令
		 */
		function system(
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

						static Value: 'm_1' | 'm_1a';

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
						function process_whole(
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
						function process_whole(
							data: CByteListView,
							value: ByteArray,
						): Void;

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
						 * 计算成品数据大小
						 * @param raw_size 原始数据大小
						 * @param ripe_size 成品数据大小
						 */
						function compute_size(
							raw_size: Size,
							ripe_size: Size,
						): Void;

						/**
						 * 编码
						 * @param raw 原始数据
						 * @param ripe 成品数据
						 */
						function process_whole(
							raw: IByteStreamView,
							ripe: OCharacterStreamView,
						): Void;

					}

					/** 解码 */
					namespace Decode {

						/**
						 * 计算原始数据大小
						 * @param ripe 成品数据
						 * @param raw_size 原始数据大小
						 */
						function compute_size(
							ripe: CCharacterListView,
							raw_size: Size,
						): Void;

						/**
						 * 解码
						 * @param ripe 成品数据
						 * @param raw 原始数据
						 */
						function process_whole(
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
					namespace Encrypt {

						/**
						 * 加密
						 * @param plain 明文数据
						 * @param cipher 密文数据
						 * @param key 密钥
						 */
						function process_whole(
							plain: IByteStreamView,
							cipher: OByteStreamView,
							key: Byte,
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
						 * @param block_size 块大小，允许16、24、32
						 * @param key_size 密钥大小，允许16、24、32
						 * @param key 密钥，大小必须与key_size一致
						 * @param iv 初始向量，当模式为cbc或cfb时有效，此时大小必须与block_size一致
						 */
						function process_whole(
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
						 * @param block_size 块大小，允许16、24、32
						 * @param key_size 密钥大小，允许16、24、32
						 * @param key 密钥，大小必须与key_size一致
						 * @param iv 初始向量，当模式为cbc或cfb时有效，此时大小必须与block_size一致
						 */
						function process_whole(
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
			namespace Compress {

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
						 * 计算成品数据大小上限
						 * @param raw_size 原始数据大小
						 * @param ripe_size_bound 成品数据大小上限
						 * @param window_bits 窗口大小(8~15)
						 * @param memory_level 内存级别(1~9)
						 * @param wrapper 封装
						 */
						function compute_size_bound(
							raw_size: Size,
							ripe_size_bound: Size,
							window_bits: Size,
							memory_level: Size,
							wrapper: Wrapper,
						): Void;

						/**
						 * 压缩
						 * @param raw 原始数据
						 * @param ripe 成品数据
						 * @param level 压缩级别(0~9)
						 * @param window_bits 窗口大小(8~15)
						 * @param memory_level 内存级别(1~9)
						 * @param strategy 策略
						 * @param wrapper 封装
						 */
						function process_whole(
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
						 * @param ripe 成品数据
						 * @param raw 原始数据
						 * @param window_bits 窗口大小(8~15)
						 * @param wrapper 封装
						 */
						function process_whole(
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
						 * @param block_size 块大小(1~9)
						 * @param work_factor 工作因子(0~250，0==30)
						 */
						function process_whole(
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
						function process_whole(
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
						 * @param property 属性数据
						 * @param level 压缩级别(0~9)
						 */
						function process_whole(
							raw: IByteStreamView,
							ripe: OByteStreamView,
							property: OByteStreamView,
							level: Size,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param ripe 成品数据
						 * @param raw 原始数据
						 * @param property 属性数据
						 */
						function process_whole(
							ripe: IByteStreamView,
							raw: OByteStreamView,
							property: IByteStreamView,
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
						 * @param disable_trailing_comma 禁用尾随逗号
						 * @param disable_array_wrap_line 禁用数组元素换行
						 */
						function process_whole<Constraint extends Core.JSON.JS_Value>(
							data: OCharacterStreamView,
							value: Core.JSON.Value<Constraint>,
							disable_trailing_comma: Boolean,
							disable_array_wrap_line: Boolean,
						): Void;

					}

					/** 读 */
					namespace Read {

						/**
						 * 读
						 * @param data 数据
						 * @param value 值
						 */
						function process_whole<Constraint extends Core.JSON.JS_Value>(
							data: ICharacterStreamView,
							value: Core.JSON.Value<Constraint>,
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
						function process_whole(
							data: String,
							value: Core.XML.Node,
						): Void;

					}

					/** 读 */
					namespace Read {

						/**
						 * 读
						 * @param data 数据
						 * @param value 值
						 */
						function process_whole(
							data: String,
							value: Core.XML.Node,
						): Void;

					}

				}

			}

		}

		/** 纹理 */
		namespace Texture {

			// ------------------------------------------------

			/** 格式 */
			class Format {

				// ------------------------------------------------

				private _Tool_Texture_Format;

				// ------------------------------------------------

				static default(): Format;

				static copy(it: Format): Format;

				// ------------------------------------------------

				static Value: 'a_8' | 'rgb_888' | 'rgba_8888' | 'rgb_565_l' | 'rgba_4444_l' | 'rgba_5551_l' | 'argb_4444_l' | 'argb_8888_l';

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
				function process_image(
					data: OByteStreamView,
					image: Image.CBitmapView,
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
				function process_image(
					data: IByteStreamView,
					image: Image.VBitmapView,
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
						function process_image(
							data: OByteStreamView,
							image: Image.CBitmapView,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param data 数据
						 * @param image 图像
						 */
						function process_image(
							data: IByteStreamView,
							image: Image.VBitmapView,
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
						 * @param with_alpha 是否携带alpha通道
						 */
						function process_image(
							data: OByteStreamView,
							image: Image.CBitmapView,
							with_alpha: Boolean,
						): Void;

					}

					/** 解压 */
					namespace Uncompress {

						/**
						 * 解压
						 * @param data 数据
						 * @param image 图像
						 * @param with_alpha 是否携带alpha通道
						 */
						function process_image(
							data: IByteStreamView,
							image: Image.VBitmapView,
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
					 * @param raw 解码后的音频数据
					 * @param ffmpeg_program ffmpeg程序文件的路径，pcm、adpcm、aac、vorbis音频解码时需要调用该程序
					 * @param ww2ogg_program ww2ogg程序文件的路径，vorbis音频解码时需要调用该程序
					 * @param ww2ogg_code_book ww2ogg代码本文件的路径，vorbis音频解码时需要使用该文件
					 * @param temporary_directory 临时文件目录
					 */
					function process_audio(
						ripe: CByteListView,
						raw: ByteArray,
						ffmpeg_program: Path,
						ww2ogg_program: Path,
						ww2ogg_code_book: Path,
						temporary_directory: Path,
					): Void;

				}

			}

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
						number: 88n | 112n | 140n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 清单 */
				namespace Manifest {

					namespace JS_N {

						/** 声音库 */
						type SoundBank = {
							/** ID */
							id: bigint;
							/** 文件头拓展数据 */
							header_expand: string;
						};

					}

					/** 声音库 */
					class SoundBank {

						// ------------------------------------------------

						private _Tool_Wwise_SoundBank_Manifest_SoundBank;

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
					 * @param sound_bank_data 声音库数据
					 * @param sound_bank_manifest 声音库清单
					 * @param embedded_audio_directory 内嵌音频目录
					 * @param version 版本
					 */
					function process_sound_bank(
						sound_bank_data: OByteStreamView,
						sound_bank_manifest: Manifest.SoundBank,
						embedded_audio_directory: Path,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param sound_bank_data 声音库数据
					 * @param sound_bank_manifest 声音库清单
					 * @param embedded_audio_directory 内嵌音频目录
					 * @param version 版本
					 */
					function process_sound_bank(
						sound_bank_data: IByteStreamView,
						sound_bank_manifest: Manifest.SoundBank,
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
					class Package {

						// ------------------------------------------------

						private _Tool_Marmalade_DZip_Manifest_Package;

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
					 * @param package_data 包
					 * @param package_manifest 清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process_package(
						package_data: OByteStreamView,
						package_manifest: Manifest.Package,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param package_data 包
					 * @param package_manifest 清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process_package(
						package_data: IByteStreamView,
						package_manifest: Manifest.Package,
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

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_ZLib_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						variant_64: boolean;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

					// ------------------------------------------------

				}

				/** 压缩 */
				namespace Compress {

					/**
					 * 计算成品数据大小上限
					 * @param raw_size 原始数据大小
					 * @param ripe_size_bound 成品数据大小上限
					 * @param window_bits 窗口大小(8~15)
					 * @param memory_level 内存级别(1~9)
					 * @param version 版本
					 */
					function compute_size_bound(
						raw_size: Size,
						ripe_size_bound: Size,
						window_bits: Size,
						memory_level: Size,
						version: Version,
					): Void;

					/**
					 * 压缩
					 * @param raw 原始数据
					 * @param ripe 成品数据
					 * @param level 压缩级别(0~9)
					 * @param window_bits 窗口大小(8~15)
					 * @param memory_level 内存级别(1~9)
					 * @param strategy 策略
					 * @param version 版本
					 */
					function process_whole(
						raw: IByteStreamView,
						ripe: OByteStreamView,
						level: Size,
						window_bits: Size,
						memory_level: Size,
						strategy: Data.Compress.Deflate.Strategy,
						version: Version,
					): Void;

				}

				/** 解压 */
				namespace Uncompress {

					/**
					 * 计算原始数据大小
					 * @param ripe 成品数据
					 * @param raw_size 原始数据大小
					 * @param version 版本
					 */
					function compute_size(
						ripe: CByteListView,
						raw_size: Size,
						version: Version,
					): Void;

					/**
					 * 解压
					 * @param ripe 成品数据
					 * @param raw 原始数据
					 * @param window_bits 窗口大小(8~15)
					 * @param version 版本
					 */
					function process_whole(
						ripe: IByteStreamView,
						raw: OByteStreamView,
						window_bits: Size,
						version: Version,
					): Void;

				}

			}

			/** Reanim */
			namespace Reanim {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_Reanim_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						platform: 'desktop' | 'phone';
						variant_64: boolean;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

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
					class Animation {

						// ------------------------------------------------

						private _Tool_PopCap_Reanim_Manifest_Animation;

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
					 * @param animation_data 动画数据
					 * @param animation_manifest 动画清单
					 * @param version 版本
					 */
					function process_animation(
						animation_data: OByteStreamView,
						animation_manifest: Manifest.Animation,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param animation_data 动画数据
					 * @param animation_manifest 动画清单
					 * @param version 版本
					 */
					function process_animation(
						animation_data: IByteStreamView,
						animation_manifest: Manifest.Animation,
						version: Version,
					): Void;

				}

			}

			/** RTON */
			namespace RTON {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_RTON_Version;

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

				/** RTON所能存储的JSON值类型 */
				type JS_ValidValue = boolean | number | bigint | string | JS_ValidValue[] | { [key: string]: JS_ValidValue; };

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param value 值
					 * @param enable_string_index 启用字符串索引，若是，确保同一字符串只会明文编码一次，之后只使用索引值，可减少编码出的rton的大小
					 * @param enable_rtid 启用rtid，若是，符合RTID格式的字符串将编码为0x83系列的rton值单元
					 * @param version 版本
					 */
					function process_whole(
						data: OByteStreamView,
						value: JSON.Value<JS_ValidValue>,
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
					 * @param value 值
					 * @param version 版本
					 */
					function process_whole(
						data: IByteStreamView,
						value: JSON.Value<JS_ValidValue>,
						version: Version,
					): Void;

				}

				/** 加密 */
				namespace Encrypt {

					/**
					 * 计算密文数据的大小
					 * @param plain_size 明文数据大小
					 * @param cipher_size 密文数据的大小
					 */
					function compute_size(
						plain_size: Size,
						cipher_size: Size,
					): Void;

					/**
					 * 加密
					 * @param plain 明文数据
					 * @param cipher 密文数据
					 * @param key 密钥
					 */
					function process_whole(
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
					 * @param plain_size 明文数据的大小
					 */
					function compute_size(
						cipher_size: Size,
						plain_size: Size,
					): Void;

					/**
					 * 解密
					 * @param cipher 密文数据
					 * @param plain 明文数据
					 * @param key 密钥
					 */
					function process_whole(
						cipher: IByteStreamView,
						plain: OByteStreamView,
						key: String,
					): Void;

				}

			}

			/** PAM */
			namespace PAM {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_PAM_Version;

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
						type Command = [
							/** 命令 */
							string,
							/** 参数 */
							string,
						];

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
					class Animation {

						// ------------------------------------------------

						private _Tool_PopCap_PAM_Manifest_Animation;

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
					 * @param animation_data 动画数据
					 * @param animation_manifest 动画清单
					 * @param version 版本
					 */
					function process_animation(
						animation_data: OByteStreamView,
						animation_manifest: Manifest.Animation,
						version: Version,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param animation_data 动画数据
					 * @param animation_manifest 动画清单
					 * @param version 版本
					 */
					function process_animation(
						animation_data: IByteStreamView,
						animation_manifest: Manifest.Animation,
						version: Version,
					): Void;

				}

			}

			/** PAK */
			namespace PAK {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_PAK_Version;

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
					class Package {

						// ------------------------------------------------

						private _Tool_PopCap_PAK_Manifest_Package;

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
					 * @param package_data 包数据
					 * @param package_manifest 包清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process_package(
						package_data: OByteStreamView,
						package_manifest: Manifest.Package,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param package_data 包数据
					 * @param package_manifest 包清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process_package(
						package_data: IByteStreamView,
						package_manifest: Manifest.Package,
						resource_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

			/** RSGP数据包 */
			namespace RSGP {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_RSGP_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 3n | 4n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

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
							value: GenericResourceAdditional;
						} | {
							type: 'texture';
							value: TextureResourceAdditional;
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
					class Package {

						// ------------------------------------------------

						private _Tool_PopCap_RSGP_Manifest_Package;

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
					 * @param package_data 包数据
					 * @param package_manifest 包清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process_package(
						package_data: OByteStreamView,
						package_manifest: Manifest.Package,
						resource_directory: Path,
						version: Version,
					): Void;

				}

				/** 解包 */
				namespace Unpack {

					/**
					 * 解包
					 * @param package_data 包数据
					 * @param package_manifest 包清单
					 * @param resource_directory 资源目录
					 * @param version 版本
					 */
					function process_package(
						package_data: IByteStreamView,
						package_manifest: Manifest.Package,
						resource_directory: PathOptional,
						version: Version,
					): Void;

				}

			}

			/** RSB数据包 */
			namespace RSB {

				/** 版本 */
				class Version {

					// ------------------------------------------------

					private _Tool_PopCap_RSB_Version;

					// ------------------------------------------------

					static default(): Version;

					static copy(it: Version): Version;

					// ------------------------------------------------

					static Value: {
						number: 3n | 4n;
						additional_texture_information_for_pvz_2_chinese_android: 0n | 1n | 2n;
					};

					static value(it: typeof Version.Value): Version;

					get value(): typeof Version.Value;

					set value(it: typeof Version.Value);

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
							value: GenericResourceAdditional;
						} | {
							type: 'texture';
							value: TextureResourceAdditional;
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
					class Package {

						// ------------------------------------------------

						private _Tool_PopCap_RSB_Manifest_Package;

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
				namespace Description {

					namespace JS_N {
					}

					/** 包 */
					class PackageOptional {

						// ------------------------------------------------

						private _Tool_PopCap_RSB_Description_PackageOptional;

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
					 * @param package_data 包数据
					 * @param package_manifest 包清单
					 * @param package_description 包详单
					 * @param resource_directory 资源目录
					 * @param packet_file 子包文件
					 * @param new_packet_file 新生成子包文件
					 * @param version 版本
					 */
					function process_package(
						package_data: OByteStreamView,
						package_manifest: Manifest.Package,
						package_description: Description.PackageOptional,
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
					 * @param package_data 包数据
					 * @param package_manifest 包清单
					 * @param package_description 包详单
					 * @param resource_directory 资源目录
					 * @param packet_file 子包文件
					 * @param version 版本
					 */
					function process_package(
						package_data: IByteStreamView,
						package_manifest: Manifest.Package,
						package_description: Description.PackageOptional,
						resource_directory: PathOptional,
						packet_file: PathOptional,
						version: Version,
					): Void;

				}

			}

		}

		/** 杂项 */
		namespace Miscellaneous {

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
					function process_image(
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
					function process_image(
						data: IByteStreamView,
						image: Image.VBitmapView,
						format: Texture.Format,
					): Void;

				}

			}

			/** PvZ2-安卓中文版RSB中的纹理alpha映射 */
			namespace PvZ2CHSRSBTextureAlphaIndex {

				type JS_IndexList = Array<bigint>;

				/** 编码 */
				namespace Encode {

					/**
					 * 编码
					 * @param data 数据
					 * @param image 图像
					 * @param index_map alpha索引
					 */
					function process_image(
						data: OByteStreamView,
						image: Image.CBitmapView,
						index_map: JS_IndexList,
					): Void;

				}

				/** 解码 */
				namespace Decode {

					/**
					 * 解码
					 * @param data 数据
					 * @param image 图像
					 * @param index_map alpha索引
					 */
					function process_image(
						data: IByteStreamView,
						image: Image.VBitmapView,
						index_map: JS_IndexList,
					): Void;

				}

			}

		}

	}

	// ------------------------------------------------

	/** 杂项 */
	namespace Miscellaneous {

		/** 上下文 */
		class Thread {

			// ------------------------------------------------

			private _Thread;

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
			 */
			join(
			): Void;

			/**
			 * 分离线程
			 */
			detach(
			): Void;

			// ------------------------------------------------

			/**
			 * 当前线程放弃目前的执行，以允许其他线程执行
			 */
			static yield(
			): Void;

			/**
			 * 当前线程休眠
			 * @param duration 休眠时间，单位为毫秒
			 */
			static sleep(
				duration: Size,
			): Void;

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
			 * @returns 结果
			 */
			shell_callback(
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
			 * @param executor 函数
			 * @param thread 线程
			 */
			execute(
				executor: () => any,
				thread: Thread,
			): Void;

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

		/** 当前上下文对象 */
		const g_context: Context;

		/** 版本编号 */
		const g_version: Size;

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
