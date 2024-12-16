namespace Twinning.Script.KernelX {

	// ------------------------------------------------

	export const system = Kernel.Miscellaneous.g_system.value;

	export const is_windows = system === 'windows';

	export const is_linux = system === 'linux';

	export const is_macintosh = system === 'macintosh';

	export const is_android = system === 'android';

	export const is_iphone = system === 'iphone';

	// ------------------------------------------------

	export const g_common_buffer = Kernel.ByteArray.default();

	export const g_common_buffer_x = Kernel.ByteArray.allocate(Kernel.Size.value(0x2000n));

	// ------------------------------------------------

	export namespace JSON {

		// ------------------------------------------------

		export const g_format: {
			disable_array_trailing_comma: boolean;
			disable_array_line_breaking: boolean;
			disable_object_trailing_comma: boolean;
			disable_object_line_breaking: boolean;
		} = {
			disable_array_trailing_comma: false,
			disable_array_line_breaking: false,
			disable_object_trailing_comma: false,
			disable_object_line_breaking: false,
		};

		// ------------------------------------------------

		export function read<Constraint extends Kernel.JSON.JS_Value>(
			data: ArrayBuffer,
		): Kernel.JSON.Value<Constraint> {
			let data_stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(Kernel.ByteListView.value(data)));
			let value = Kernel.JSON.Value.default<Constraint>();
			let buffer_stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer_x.view()));
			Kernel.Tool.Data.Serialization.JSON.Read.process(data_stream, value, buffer_stream);
			return value;
		}

		/** NOTE : result is a view of buffer */
		export function write<Constraint extends Kernel.JSON.JS_Value>(
			value: Kernel.JSON.Value<Constraint>,
			disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
			disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
			disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
			disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
			data_buffer: Kernel.CharacterListView | bigint = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): ArrayBuffer {
			let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
			let data_view = data_buffer instanceof Kernel.CharacterListView ? data_buffer : Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(data!.view());
			let data_stream = Kernel.CharacterStreamView.watch(data_view);
			Kernel.Tool.Data.Serialization.JSON.Write.process(data_stream, value, Kernel.Boolean.value(disable_array_trailing_comma), Kernel.Boolean.value(disable_array_line_breaking), Kernel.Boolean.value(disable_object_trailing_comma), Kernel.Boolean.value(disable_object_line_breaking));
			return Kernel.Miscellaneous.cast_CharacterListView_to_ByteListView(data_stream.stream_view()).value;
		}

		// ------------------------------------------------

		export function read_s<Constraint extends Kernel.JSON.JS_Value>(
			data: string,
		): Kernel.JSON.Value<Constraint> {
			let data_byte = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(data));
			return read(data_byte.view().value);
		}

		export function write_s<Constraint extends Kernel.JSON.JS_Value>(
			value: Kernel.JSON.Value<Constraint>,
			disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
			disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
			disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
			disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
			data_buffer: Kernel.CharacterListView | bigint = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): string {
			let data = write(value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
			return Kernel.Miscellaneous.cast_CharacterListView_to_JS_String(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(Kernel.ByteListView.value(data)));
		}

		// ------------------------------------------------

		export function read_fs<Constraint extends Kernel.JSON.JS_Value>(
			data_file: string,
		): Kernel.JSON.Value<Constraint> {
			let data = Storage.read_file(data_file);
			return read(data.view().value);
		}

		export function write_fs<Constraint extends Kernel.JSON.JS_Value>(
			data_file: string,
			value: Kernel.JSON.Value<Constraint>,
			disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
			disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
			disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
			disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
			data_buffer: Kernel.CharacterListView | bigint = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): void {
			let data = write(value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
			Storage.write_file(data_file, data);
			return;
		}

		// ------------------------------------------------

		export function read_js<Constraint extends Kernel.JSON.JS_Value>(
			data: ArrayBuffer,
		): Constraint {
			return read<Constraint>(data).value;
		}

		/** NOTE : result is a view of buffer */
		export function write_js<Constraint extends Kernel.JSON.JS_Value>(
			value: Constraint,
			disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
			disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
			disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
			disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
			data_buffer: Kernel.CharacterListView | bigint = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): ArrayBuffer {
			return write(Kernel.JSON.Value.value<Constraint>(value), disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
		}

		// ------------------------------------------------

		export function read_s_js<Constraint extends Kernel.JSON.JS_Value>(
			data: string,
		): Constraint {
			return read_s<Constraint>(data).value;
		}

		export function write_s_js<Constraint extends Kernel.JSON.JS_Value>(
			value: Constraint,
			disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
			disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
			disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
			disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
			data_buffer: Kernel.CharacterListView | bigint = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): string {
			return write_s(Kernel.JSON.Value.value<Constraint>(value), disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
		}

		// ------------------------------------------------

		export function read_fs_js<Constraint extends Kernel.JSON.JS_Value>(
			data_file: string,
		): Constraint {
			return read_fs<Constraint>(data_file).value;
		}

		export function write_fs_js<Constraint extends Kernel.JSON.JS_Value>(
			data_file: string,
			value: Constraint,
			disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
			disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
			disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
			disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
			data_buffer: Kernel.CharacterListView | bigint = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): void {
			return write_fs(data_file, Kernel.JSON.Value.value<Constraint>(value), disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
		}

		// ------------------------------------------------

	}

	export namespace XML {

		// ------------------------------------------------

		export function read_fs(
			data_file: string,
		): Kernel.XML.Node {
			let data_byte = Storage.read_file(data_file);
			let data = Kernel.Miscellaneous.cast_moveable_ByteArray_to_String(data_byte);
			let value = Kernel.XML.Node.default();
			Kernel.Tool.Data.Serialization.XML.Read.process(data, value);
			return value;
		}

		export function write_fs(
			data_file: string,
			value: Kernel.XML.Node,
		): void {
			let data = Kernel.String.default();
			Kernel.Tool.Data.Serialization.XML.Write.process(data, value);
			let data_byte = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(data);
			Storage.write_file(data_file, data_byte.view());
			return;
		}

		// ------------------------------------------------

		export function read_fs_js(
			data_file: string,
		): Kernel.XML.JS_Node {
			return read_fs(data_file).value;
		}

		export function write_fs_js(
			data_file: string,
			value: Kernel.XML.JS_Node,
		): void {
			return write_fs(data_file, Kernel.XML.Node.value(value));
		}

		// ------------------------------------------------

	}

	export namespace Image {

		// ------------------------------------------------

		export type ImageSize = typeof Kernel.Image.ImageSize.Value;

		export type ImagePosition = typeof Kernel.Image.ImagePosition.Value;

		// ------------------------------------------------

		export type Color = typeof Kernel.Image.Color.Value;

		export type ColorList = typeof Kernel.Image.ColorList.Value;

		export type Pixel = typeof Kernel.Image.Pixel.Value;

		// ------------------------------------------------

		export namespace File {

			// ------------------------------------------------

			export namespace PNG {

				// ------------------------------------------------

				export function size(
					data: Kernel.ByteListView,
				): ImageSize {
					let size = Kernel.Image.ImageSize.default();
					Kernel.Tool.Texture.File.PNG.Read.estimate(data, size);
					return size.value;
				}

				export function read(
					data: Kernel.ByteStreamView,
					image: Kernel.Image.VImageView,
				): void {
					return Kernel.Tool.Texture.File.PNG.Read.process(data, image);
				}

				export function write(
					data: Kernel.ByteStreamView,
					image: Kernel.Image.CImageView,
				): void {
					return Kernel.Tool.Texture.File.PNG.Write.process(data, image);
				}

				// ------------------------------------------------

				// NOTE : avoid use this function
				export function size_fs(
					file: string,
				): ImageSize {
					let data = Storage.read_file(file);
					let image_size = size(data.view());
					return image_size;
				}

				export function read_fs(
					file: string,
					image: Kernel.Image.VImageView,
				): void {
					let data = Storage.read_file(file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					read(data_stream, image);
					return;
				}

				export function write_fs(
					file: string,
					image: Kernel.Image.CImageView,
					data_buffer: Kernel.ByteListView | bigint = g_common_buffer.view(),
				): void {
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_view = data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view();
					let data_stream = Kernel.ByteStreamView.watch(data_view);
					write(data_stream, image);
					Storage.write_file(file, data_stream.stream_view());
					return;
				}

				export function read_fs_of(
					file: string,
				): Kernel.Image.Image {
					let data = Storage.read_file(file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image_size = size(data.view());
					let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(image_size));
					read(data_stream, image.view());
					return image;
				}

				// ------------------------------------------------

			}

			// ------------------------------------------------

		}

		// ------------------------------------------------

	}

	export namespace Storage {

		// ------------------------------------------------

		export function exist(
			target: string,
		): boolean {
			return Kernel.Storage.exist(Kernel.Path.value(target)).value;
		}

		export function exist_file(
			target: string,
		): boolean {
			return Kernel.Storage.exist_file(Kernel.Path.value(target)).value;
		}

		export function exist_directory(
			target: string,
		): boolean {
			return Kernel.Storage.exist_directory(Kernel.Path.value(target)).value;
		}

		// ------------------------------------------------

		export function copy(
			source: string,
			destination: string,
		): void {
			return Kernel.Storage.copy(Kernel.Path.value(source), Kernel.Path.value(destination));
		}

		export function rename(
			source: string,
			destination: string,
		): void {
			return Kernel.Storage.rename(Kernel.Path.value(source), Kernel.Path.value(destination));
		}

		export function remove(
			source: string,
		): void {
			return Kernel.Storage.remove(Kernel.Path.value(source));
		}

		// ------------------------------------------------

		export function create_link(
			target: string,
			object: string,
			is_directory: boolean,
		): void {
			return Kernel.Storage.create_link(Kernel.Path.value(target), Kernel.Path.value(object), Kernel.Boolean.value(is_directory));
		}

		export function parse_link(
			target: string,
		): string {
			return Kernel.Storage.parse_link(Kernel.Path.value(target)).value;
		}

		// ------------------------------------------------

		export function create_hard_link(
			target: string,
			object: string,
		): void {
			return Kernel.Storage.create_hard_link(Kernel.Path.value(target), Kernel.Path.value(object));
		}

		// ------------------------------------------------

		export function create_file(
			target: string,
		): void {
			return Kernel.Storage.create_file(Kernel.Path.value(target));
		}

		// ------------------------------------------------

		export function size_file(
			target: string,
		): bigint {
			return Kernel.Storage.size_file(Kernel.Path.value(target)).value;
		}

		export function resize_file(
			target: string,
			size: bigint,
		): void {
			return Kernel.Storage.resize_file(Kernel.Path.value(target), Kernel.Size.value(size));
		}

		// ------------------------------------------------

		export function read_file(
			target: string,
		): Kernel.ByteArray {
			return Kernel.Storage.read_file(Kernel.Path.value(target));
		}

		export function write_file(
			target: string,
			data: Kernel.ByteListView | Kernel.ByteArray | ArrayBuffer,
		): void {
			let data_view: Kernel.ByteListView;
			if (data instanceof Kernel.ByteListView) {
				data_view = data;
			}
			if (data instanceof Kernel.ByteArray) {
				data_view = data.view();
			}
			if (data instanceof ArrayBuffer) {
				data_view = Kernel.ByteListView.value(data);
			}
			return Kernel.Storage.write_file(Kernel.Path.value(target), data_view!);
		}

		export function read_file_s(
			target: string,
		): string {
			let data = read_file(target);
			let data_string = Kernel.Miscellaneous.cast_CharacterListView_to_JS_String(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(data.view()));
			return data_string;
		}

		export function write_file_s(
			target: string,
			data: string,
		): void {
			let data_byte = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(data));
			write_file(target, data_byte);
			return;
		}

		// ------------------------------------------------

		export function create_directory(
			path: string,
		): void {
			return Kernel.Storage.create_directory(Kernel.Path.value(path));
		}

		// ------------------------------------------------

		export function count(
			target: string,
			depth: null | bigint = null,
		): bigint {
			return Kernel.Storage.count(Kernel.Path.value(target), Kernel.SizeOptional.value(depth)).value;
		}

		export function count_file(
			target: string,
			depth: null | bigint = null,
		): bigint {
			return Kernel.Storage.count_file(Kernel.Path.value(target), Kernel.SizeOptional.value(depth)).value;
		}

		export function count_directory(
			target: string,
			depth: null | bigint = null,
		): bigint {
			return Kernel.Storage.count_directory(Kernel.Path.value(target), Kernel.SizeOptional.value(depth)).value;
		}

		// ------------------------------------------------

		export function list(
			target: string,
			depth: null | bigint = null,
		): Array<string> {
			return Kernel.Storage.list(Kernel.Path.value(target), Kernel.SizeOptional.value(depth)).value;
		}

		export function list_file(
			target: string,
			depth: null | bigint = null,
		): Array<string> {
			return Kernel.Storage.list_file(Kernel.Path.value(target), Kernel.SizeOptional.value(depth)).value;
		}

		export function list_directory(
			target: string,
			depth: null | bigint = null,
		): Array<string> {
			return Kernel.Storage.list_directory(Kernel.Path.value(target), Kernel.SizeOptional.value(depth)).value;
		}

		// ------------------------------------------------

	}

	export namespace Process {

		// ------------------------------------------------

		export function get_working_directory(
		): string {
			return Kernel.Process.get_working_directory().value;
		}

		export function set_working_directory(
			target: string,
		): void {
			return Kernel.Process.set_working_directory(Kernel.Path.value(target));
		}

		// ------------------------------------------------

		export function get_environment_variable(
			name: string,
		): null | string {
			return Kernel.Process.get_environment_variable(Kernel.String.value(name)).value;
		}

		export function set_environment_variable(
			name: string,
			value: null | string,
		): void {
			return Kernel.Process.set_environment_variable(Kernel.String.value(name), Kernel.StringOptional.value(value));
		}

		export function list_environment_variable(
		): Array<string> {
			return Kernel.Process.list_environment_variable().value;
		}

		// ------------------------------------------------

		export function spawn_child(
			program: string,
			argument: Array<string>,
			environment: Array<string>,
			input: null | string,
			output: null | string,
			error: null | string,
		): bigint {
			return Kernel.Process.spawn_child(Kernel.Path.value(program), Kernel.StringList.value(argument), Kernel.StringList.value(environment), Kernel.PathOptional.value(input), Kernel.PathOptional.value(output), Kernel.PathOptional.value(error)).value;
		}

		// ------------------------------------------------

		export function execute_system_command(
			command: string,
		): bigint {
			return Kernel.Process.execute_system_command(Kernel.String.value(command)).value;
		}

		// ------------------------------------------------

	}

	export namespace Tool {

		export namespace Data {

			export namespace Hash {

				export namespace MD5 {

					export function hash_fs(
						data_file: string,
					): bigint {
						let data = Storage.read_file(data_file);
						let value = Kernel.ByteArray.default();
						Kernel.Tool.Data.Hash.MD5.Hash.process(data.view(), value);
						return integer_from_byte_array([...new Uint8Array(value.value)].map(BigInt), null, 'little');
					}

				}

				export namespace FNV {

					export function hash_fs(
						data_file: string,
						mode: typeof Kernel.Tool.Data.Hash.FNV.Mode.Value,
						bit_count: typeof Kernel.Tool.Data.Hash.FNV.BitCount.Value,
					): bigint {
						let data = Storage.read_file(data_file);
						let value = Kernel.ByteArray.default();
						Kernel.Tool.Data.Hash.FNV.Hash.process(data.view(), value, Kernel.Tool.Data.Hash.FNV.Mode.value(mode), Kernel.Tool.Data.Hash.FNV.BitCount.value(bit_count));
						return integer_from_byte_array([...new Uint8Array(value.value)].map(BigInt), null, 'current');
					}

					export function hash_s(
						data_string: string,
						mode: typeof Kernel.Tool.Data.Hash.FNV.Mode.Value,
						bit_count: typeof Kernel.Tool.Data.Hash.FNV.BitCount.Value,
					): bigint {
						let data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(data_string));
						let value = Kernel.ByteArray.default();
						Kernel.Tool.Data.Hash.FNV.Hash.process(data.view(), value, Kernel.Tool.Data.Hash.FNV.Mode.value(mode), Kernel.Tool.Data.Hash.FNV.BitCount.value(bit_count));
						return integer_from_byte_array([...new Uint8Array(value.value)].map(BigInt), null, 'current');
					}

				}

			}

			export namespace Encoding {

				export namespace Base64 {

					export function encode_fs(
						raw_file: string,
						ripe_file: string,
					): void {
						let raw = Storage.read_file(raw_file);
						let ripe_size = Kernel.Size.default();
						Kernel.Tool.Data.Encoding.Base64.Encode.estimate(raw.size(), ripe_size);
						let ripe = Kernel.ByteArray.allocate(ripe_size);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe_stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(ripe.view()));
						Kernel.Tool.Data.Encoding.Base64.Encode.process(raw_stream, ripe_stream);
						Storage.write_file(ripe_file, Kernel.Miscellaneous.cast_CharacterListView_to_ByteListView(ripe_stream.stream_view()));
						return;
					}

					export function decode_fs(
						ripe_file: string,
						raw_file: string,
					): void {
						let ripe = Storage.read_file(ripe_file);
						let ripe_stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(ripe.view()));
						let raw_size = Kernel.Size.default();
						Kernel.Tool.Data.Encoding.Base64.Decode.estimate(ripe_stream.view(), raw_size);
						let raw = Kernel.ByteArray.allocate(raw_size);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						Kernel.Tool.Data.Encoding.Base64.Decode.process(ripe_stream, raw_stream);
						Storage.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

			}

			export namespace Encryption {

				export namespace EXOR {

					export function encrypt_fs(
						plain_file: string,
						cipher_file: string,
						key: Array<bigint>,
					): void {
						let plain = Storage.read_file(plain_file);
						let plain_stream = Kernel.ByteStreamView.watch(plain.view());
						let cipher = Kernel.ByteArray.allocate(plain.size());
						let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
						let key_c = Kernel.ByteArray.allocate(Kernel.Size.value(BigInt(key.length)));
						new Uint8Array(key_c.view().value).set(key.map(Number));
						Kernel.Tool.Data.Encryption.EXOR.Encrypt.process(plain_stream, cipher_stream, key_c.view());
						Storage.write_file(cipher_file, cipher_stream.stream_view());
						return;
					}

				}

				export namespace Rijndael {

					const ModeX = ['ecb', 'cbc', 'cfb'] as const;

					const BlockSizeX = [16n, 24n, 32n] as const;

					export type Mode = typeof ModeX[number];

					export const ModeE = ModeX as unknown as Mode[];

					export type BlockSize = typeof BlockSizeX[number];

					export const BlockSizeE = BlockSizeX as unknown as BlockSize[];

					export function encrypt_fs(
						plain_file: string,
						cipher_file: string,
						mode: Mode,
						block_size: BlockSize,
						key_size: BlockSize,
						key: string,
						iv: string,
					): void {
						let plain = Storage.read_file(plain_file);
						let plain_stream = Kernel.ByteStreamView.watch(plain.view());
						let cipher = Kernel.ByteArray.allocate(plain.size());
						let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
						Kernel.Tool.Data.Encryption.Rijndael.Encrypt.process(plain_stream, cipher_stream, Kernel.Tool.Data.Encryption.Rijndael.Mode.value(mode), Kernel.Size.value(block_size), Kernel.Size.value(key_size), Kernel.String.value(key), Kernel.String.value(iv));
						Storage.write_file(cipher_file, cipher_stream.stream_view());
						return;
					}

					export function decrypt_fs(
						cipher_file: string,
						plain_file: string,
						mode: Mode,
						block_size: BlockSize,
						key_size: BlockSize,
						key: string,
						iv: string,
					): void {
						let cipher = Storage.read_file(cipher_file);
						let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
						let plain = Kernel.ByteArray.allocate(cipher.size());
						let plain_stream = Kernel.ByteStreamView.watch(plain.view());
						Kernel.Tool.Data.Encryption.Rijndael.Decrypt.process(cipher_stream, plain_stream, Kernel.Tool.Data.Encryption.Rijndael.Mode.value(mode), Kernel.Size.value(block_size), Kernel.Size.value(key_size), Kernel.String.value(key), Kernel.String.value(iv));
						Storage.write_file(plain_file, plain_stream.stream_view());
						return;
					}

				}

			}

			export namespace Compression {

				export namespace Deflate {

					const LevelX = [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					const WindowBitsX = [8n, 9n, 10n, 11n, 12n, 13n, 14n, 15n] as const;

					const MemoryLevelX = [1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					const StrategyX = ['default_mode', 'filtered', 'huffman_only', 'rle', 'fixed'] as const;

					const WrapperTypeX = ['none', 'zlib', 'gzip'] as const;

					export type Level = typeof LevelX[number];

					export const LevelE = LevelX as unknown as Level[];

					export type WindowBits = typeof WindowBitsX[number];

					export const WindowBitsE = WindowBitsX as unknown as WindowBits[];

					export type MemoryLevel = typeof MemoryLevelX[number];

					export const MemoryLevelE = MemoryLevelX as unknown as MemoryLevel[];

					export type Strategy = typeof StrategyX[number];

					export const StrategyE = StrategyX as unknown as Strategy[];

					export type WrapperType = typeof WrapperTypeX[number];

					export const WrapperTypeE = WrapperTypeX as unknown as WrapperType[];

					export function compress_fs(
						raw_file: string,
						ripe_file: string,
						level: Level,
						window_bits: WindowBits,
						memory_level: MemoryLevel,
						strategy: Strategy,
						wrapper: WrapperType,
					): void {
						let raw = Storage.read_file(raw_file);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe_size_bound = Kernel.Size.default();
						Kernel.Tool.Data.Compression.Deflate.Compress.estimate(raw.size(), ripe_size_bound, Kernel.Size.value(window_bits), Kernel.Size.value(memory_level), Kernel.Tool.Data.Compression.Deflate.Wrapper.value(wrapper));
						let ripe = Kernel.ByteArray.allocate(ripe_size_bound);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						Kernel.Tool.Data.Compression.Deflate.Compress.process(raw_stream, ripe_stream, Kernel.Size.value(level), Kernel.Size.value(window_bits), Kernel.Size.value(memory_level), Kernel.Tool.Data.Compression.Deflate.Strategy.value(strategy), Kernel.Tool.Data.Compression.Deflate.Wrapper.value(wrapper));
						Storage.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						ripe_file: string,
						raw_file: string,
						window_bits: WindowBits,
						wrapper: WrapperType,
						raw_buffer: Kernel.ByteListView | bigint,
					): void {
						let ripe = Storage.read_file(ripe_file);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let raw = is_bigint(raw_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(raw_buffer)) : null;
						let raw_stream = Kernel.ByteStreamView.watch(raw_buffer instanceof Kernel.ByteListView ? raw_buffer : raw!.view());
						Kernel.Tool.Data.Compression.Deflate.Uncompress.process(ripe_stream, raw_stream, Kernel.Size.value(window_bits), Kernel.Tool.Data.Compression.Deflate.Wrapper.value(wrapper));
						Storage.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

				export namespace BZip2 {

					const BlockSizeX = [1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type BlockSize = typeof BlockSizeX[number];

					export const BlockSizeE = BlockSizeX as unknown as BlockSize[];

					export function compress_fs(
						raw_file: string,
						ripe_file: string,
						block_size: BlockSize,
					): void {
						let raw = Storage.read_file(raw_file);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe_size_bound = Kernel.Size.value(raw.size().value + 128n); // TODO
						let ripe = Kernel.ByteArray.allocate(ripe_size_bound);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						Kernel.Tool.Data.Compression.BZip2.Compress.process(raw_stream, ripe_stream, Kernel.Size.value(block_size), Kernel.Size.value(0n));
						Storage.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						ripe_file: string,
						raw_file: string,
						raw_buffer: Kernel.ByteListView | bigint,
					): void {
						let ripe = Storage.read_file(ripe_file);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let raw = is_bigint(raw_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(raw_buffer)) : null;
						let raw_stream = Kernel.ByteStreamView.watch(raw_buffer instanceof Kernel.ByteListView ? raw_buffer : raw!.view());
						Kernel.Tool.Data.Compression.BZip2.Uncompress.process(ripe_stream, raw_stream, Kernel.Boolean.value(false));
						Storage.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

				export namespace Lzma {

					const LevelX = [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type Level = typeof LevelX[number];

					export const LevelE = LevelX as unknown as Level[];

					export function compress_fs(
						raw_file: string,
						ripe_file: string,
						level: Level,
					): void {
						let raw = Storage.read_file(raw_file);
						let ripe_size_bound = Kernel.Size.value(raw.size().value + 128n); // TODO
						let ripe = Kernel.ByteArray.allocate(ripe_size_bound);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						Kernel.Tool.Data.Compression.Lzma.Compress.process(raw_stream, ripe_stream, Kernel.Size.value(level));
						Storage.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						ripe_file: string,
						raw_file: string,
						raw_buffer: Kernel.ByteListView | bigint,
					): void {
						let ripe = Storage.read_file(ripe_file);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let raw = is_bigint(raw_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(raw_buffer)) : null;
						let raw_stream = Kernel.ByteStreamView.watch(raw_buffer instanceof Kernel.ByteListView ? raw_buffer : raw!.view());
						Kernel.Tool.Data.Compression.Lzma.Uncompress.process(ripe_stream, raw_stream);
						Storage.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

			}

			export namespace Differentiation {

				export namespace VCDiff {

					export function encode_fs(
						before_file: string,
						after_file: string,
						patch_file: string,
						interleaved: boolean,
						patch_size_bound: bigint,
					): void {
						let before = Storage.read_file(before_file);
						let before_stream = Kernel.ByteStreamView.watch(before.view());
						let after = Storage.read_file(after_file);
						let after_stream = Kernel.ByteStreamView.watch(after.view());
						let patch = Kernel.ByteArray.allocate(Kernel.Size.value(patch_size_bound));
						let patch_stream = Kernel.ByteStreamView.watch(patch.view());
						Kernel.Tool.Data.Differentiation.VCDiff.Encode.process(before_stream, after_stream, patch_stream, Kernel.Boolean.value(interleaved));
						Storage.write_file(patch_file, patch_stream.stream_view());
						return;
					}

					export function decode_fs(
						before_file: string,
						after_file: string,
						patch_file: string,
						maximum_window_size: bigint,
						after_size_bound: bigint,
					): void {
						let before = Storage.read_file(before_file);
						let before_stream = Kernel.ByteStreamView.watch(before.view());
						let after = Kernel.ByteArray.allocate(Kernel.Size.value(after_size_bound));
						let after_stream = Kernel.ByteStreamView.watch(after.view());
						let patch = Storage.read_file(patch_file);
						let patch_stream = Kernel.ByteStreamView.watch(patch.view());
						Kernel.Tool.Data.Differentiation.VCDiff.Decode.process(before_stream, after_stream, patch_stream, Kernel.Size.value(maximum_window_size));
						Storage.write_file(after_file, after_stream.stream_view());
						return;
					}

				}

			}

		}

		export namespace Texture {

			export namespace Transformation {

				export function flip(
					target: Kernel.Image.VImageView,
					horizontal: boolean,
					vertical: boolean,
				): void {
					return Kernel.Tool.Texture.Transformation.Flip.process(target, Kernel.Boolean.value(horizontal), Kernel.Boolean.value(vertical));
				}

				export function scale(
					source: Kernel.Image.CImageView,
					destination: Kernel.Image.VImageView,
				): void {
					return Kernel.Tool.Texture.Transformation.Scale.process(source, destination);
				}

				// ------------------------------------------------

				export function flip_fs(
					source_file: string,
					destination_file: string,
					horizontal: boolean,
					vertical: boolean,
				): void {
					let target = KernelX.Image.File.PNG.read_fs_of(source_file);
					let target_view = target.view();
					flip(target_view, horizontal, vertical);
					KernelX.Image.File.PNG.write_fs(destination_file, target_view);
					return;
				}

				export function scale_fs(
					source_file: string,
					destination_file: string,
					size: Image.ImageSize,
				): void {
					let source = KernelX.Image.File.PNG.read_fs_of(source_file);
					let source_view = source.view();
					let destination = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(size));
					let destination_view = destination.view();
					scale(source_view, destination_view);
					KernelX.Image.File.PNG.write_fs(destination_file, destination_view);
					return;
				}

				export function scale_rate_fs(
					source_file: string,
					destination_file: string,
					size_rate: number,
				): void {
					let source = KernelX.Image.File.PNG.read_fs_of(source_file);
					let source_view = source.view();
					let destination = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value([BigInt(Math.max(1, Math.round(Number(source.size().value[0]) * size_rate))), BigInt(Math.max(1, Math.round(Number(source.size().value[1]) * size_rate)))]));
					let destination_view = destination.view();
					scale(source_view, destination_view);
					KernelX.Image.File.PNG.write_fs(destination_file, destination_view);
					return;
				}

			}

			export namespace Encoding {

				// ------------------------------------------------

				const FormatX = [
					'a_8',
					'rgb_332',
					'rgb_565',
					'rgba_5551',
					'rgba_4444',
					'rgba_8888',
					'argb_1555',
					'argb_4444',
					'argb_8888',
					'l_8',
					'la_44',
					'la_88',
					'al_44',
					'al_88',
					'rgb_888_o',
					'rgba_8888_o',
				] as const;

				const CompressionX = [
					'rgb_etc1',
					'rgb_etc2',
					'rgba_etc2',
					'rgb_pvrtc4',
					'rgba_pvrtc4',
				] as const;

				const CompositeFormatX = [
					...FormatX,
					...CompressionX,
				] as const;

				export type Format = typeof FormatX[number];

				export const FormatE = FormatX as unknown as Format[];

				export type Compression = typeof CompressionX[number];

				export const CompressionE = CompressionX as unknown as Compression[];

				export type CompositeFormat = typeof CompositeFormatX[number];

				export const CompositeFormatE = CompositeFormatX as unknown as CompositeFormat[];

				// ------------------------------------------------

				export function get_bpp(
					format: CompositeFormat,
				): bigint {
					let result: bigint;
					switch (format) {
						case 'a_8': {
							result = 8n;
							break;
						}
						case 'rgb_332': {
							result = 8n;
							break;
						}
						case 'rgb_565': {
							result = 16n;
							break;
						}
						case 'rgba_5551': {
							result = 16n;
							break;
						}
						case 'rgba_4444': {
							result = 16n;
							break;
						}
						case 'rgba_8888': {
							result = 32n;
							break;
						}
						case 'argb_1555': {
							result = 16n;
							break;
						}
						case 'argb_4444': {
							result = 16n;
							break;
						}
						case 'argb_8888': {
							result = 32n;
							break;
						}
						case 'l_8': {
							result = 8n;
							break;
						}
						case 'la_44': {
							result = 8n;
							break;
						}
						case 'la_88': {
							result = 16n;
							break;
						}
						case 'al_44': {
							result = 8n;
							break;
						}
						case 'al_88': {
							result = 16n;
							break;
						}
						case 'rgb_888_o': {
							result = 24n;
							break;
						}
						case 'rgba_8888_o': {
							result = 32n;
							break;
						}
						case 'rgb_etc1': {
							result = 4n;
							break;
						}
						case 'rgb_etc2': {
							result = 4n;
							break;
						}
						case 'rgba_etc2': {
							result = 8n;
							break;
						}
						case 'rgb_pvrtc4': {
							result = 4n;
							break;
						}
						case 'rgba_pvrtc4': {
							result = 4n;
							break;
						}
					}
					return result;
				}

				export function compute_data_size(
					size: Image.ImageSize,
					format: CompositeFormat,
				): bigint {
					return size[0] * size[1] * get_bpp(format) / 8n;
				}

				export function compute_padded_image_size(
					origin_size: KernelX.Image.ImageSize,
					format: CompositeFormat,
				): KernelX.Image.ImageSize {
					let compute = (t: bigint) => {
						let r = 0b1n << 1n;
						while (r < t) {
							r <<= 1n;
						}
						return r;
					};
					let padded_size: KernelX.Image.ImageSize;
					if (format.includes('etc1')) {
						padded_size = [compute(origin_size[0]), compute(origin_size[1])];
					}
					else if (format.includes('pvrtc')) {
						let padded_width = compute(origin_size[0]);
						let padded_height = compute(origin_size[1]);
						let maximum_size = padded_width > padded_height ? padded_width : padded_height;
						padded_size = [maximum_size, maximum_size];
					}
					else {
						padded_size = [origin_size[0], origin_size[1]];
					}
					return padded_size;
				}

				export function is_opacity_format(
					format: CompositeFormat,
				): boolean {
					return [
						'rgb_332',
						'rgb_565',
						'l_8',
						'rgb_888_o',
						'rgb_etc1',
						'rgb_etc2',
						'rgb_pvrtc4',
					].includes(format);
				}

				// ------------------------------------------------

				export function encode(
					data: Kernel.OByteStreamView,
					image: Kernel.Image.CImageView,
					format: CompositeFormat,
				): void {
					switch (format) {
						case 'a_8':
						case 'rgb_332':
						case 'rgb_565':
						case 'rgba_5551':
						case 'rgba_4444':
						case 'rgba_8888':
						case 'argb_1555':
						case 'argb_4444':
						case 'argb_8888':
						case 'l_8':
						case 'la_44':
						case 'la_88':
						case 'al_44':
						case 'al_88':
						case 'rgb_888_o':
						case 'rgba_8888_o': {
							Kernel.Tool.Texture.Encoding.Encode.process(data, image, Kernel.Tool.Texture.Encoding.Format.value(format));
							break;
						}
						case 'rgb_etc1': {
							Kernel.Tool.Texture.Compression.ETC.Compress.process(data, image, Kernel.Tool.Texture.Compression.ETC.Format.value('v1_rgb'));
							break;
						}
						case 'rgb_etc2': {
							Kernel.Tool.Texture.Compression.ETC.Compress.process(data, image, Kernel.Tool.Texture.Compression.ETC.Format.value('v2_rgb'));
							break;
						}
						case 'rgba_etc2': {
							Kernel.Tool.Texture.Compression.ETC.Compress.process(data, image, Kernel.Tool.Texture.Compression.ETC.Format.value('v2_rgba'));
							break;
						}
						case 'rgb_pvrtc4': {
							Kernel.Tool.Texture.Compression.PVRTC.Compress.process(data, image, Kernel.Tool.Texture.Compression.PVRTC.Format.value('v1_4bpp_rgb'));
							break;
						}
						case 'rgba_pvrtc4': {
							Kernel.Tool.Texture.Compression.PVRTC.Compress.process(data, image, Kernel.Tool.Texture.Compression.PVRTC.Format.value('v1_4bpp_rgba'));
							break;
						}
					}
					return;
				}

				export function decode(
					data: Kernel.IByteStreamView,
					image: Kernel.Image.VImageView,
					format: CompositeFormat,
				): void {
					switch (format) {
						case 'a_8':
						case 'rgb_332':
						case 'rgb_565':
						case 'rgba_5551':
						case 'rgba_4444':
						case 'rgba_8888':
						case 'argb_1555':
						case 'argb_4444':
						case 'argb_8888':
						case 'l_8':
						case 'la_44':
						case 'la_88':
						case 'al_44':
						case 'al_88':
						case 'rgb_888_o':
						case 'rgba_8888_o': {
							Kernel.Tool.Texture.Encoding.Decode.process(data, image, Kernel.Tool.Texture.Encoding.Format.value(format));
							break;
						}
						case 'rgb_etc1': {
							Kernel.Tool.Texture.Compression.ETC.Uncompress.process(data, image, Kernel.Tool.Texture.Compression.ETC.Format.value('v1_rgb'));
							break;
						}
						case 'rgb_etc2': {
							Kernel.Tool.Texture.Compression.ETC.Uncompress.process(data, image, Kernel.Tool.Texture.Compression.ETC.Format.value('v2_rgb'));
							break;
						}
						case 'rgba_etc2': {
							Kernel.Tool.Texture.Compression.ETC.Uncompress.process(data, image, Kernel.Tool.Texture.Compression.ETC.Format.value('v2_rgba'));
							break;
						}
						case 'rgb_pvrtc4': {
							Kernel.Tool.Texture.Compression.PVRTC.Uncompress.process(data, image, Kernel.Tool.Texture.Compression.PVRTC.Format.value('v1_4bpp_rgb'));
							break;
						}
						case 'rgba_pvrtc4': {
							Kernel.Tool.Texture.Compression.PVRTC.Uncompress.process(data, image, Kernel.Tool.Texture.Compression.PVRTC.Format.value('v1_4bpp_rgba'));
							break;
						}
					}
					return;
				}

				// ------------------------------------------------

				export function encode_fs(
					data_file: string,
					image_file: string,
					format: CompositeFormat,
				): void {
					let image_original = KernelX.Storage.read_file(image_file);
					let image_stream = Kernel.ByteStreamView.watch(image_original.view());
					let image_size = KernelX.Image.File.PNG.size(image_stream.view());
					let image_size_padded = compute_padded_image_size(image_size, format);
					let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(image_size_padded));
					let image_view = image.view();
					KernelX.Image.File.PNG.read(image_stream, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
					let data_size = compute_data_size(image_size_padded, format);
					let data = Kernel.ByteArray.allocate(Kernel.Size.value(data_size));
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					encode(data_stream, image_view, format);
					KernelX.Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					image_file: string,
					image_size: KernelX.Image.ImageSize,
					format: CompositeFormat,
				): void {
					let data = KernelX.Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image_size_padded = compute_padded_image_size(image_size, format);
					let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(image_size_padded));
					let image_view = image.view();
					if (is_opacity_format(format)) {
						image_view.fill(Kernel.Image.Pixel.value([0xFFn, 0xFFn, 0xFFn, 0xFFn]));
					}
					decode(data_stream, image_view, format);
					KernelX.Image.File.PNG.write_fs(image_file, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
					return;
				}

				// ------------------------------------------------

			}

		}

		export namespace Wwise {

			export namespace SoundBank {

				export type VersionNumber = typeof Kernel.Tool.Wwise.SoundBank.Version.Value.number;

				export const VersionNumberE = [72n, 88n, 112n, 113n, 118n, 120n, 125n, 128n, 132n, 134n, 135n, 140n, 145n, 150n] as VersionNumber[];

				export function encode_fs(
					data_file: string,
					definition_file: string,
					embedded_media_directory: string,
					version: typeof Kernel.Tool.Wwise.SoundBank.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.Wwise.SoundBank.Version.value(version);
					let definition = Kernel.Tool.Wwise.SoundBank.Definition.SoundBank.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.Wwise.SoundBank.Encode.process(data_stream, definition, Kernel.Path.value(embedded_media_directory), version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: null | string,
					embedded_media_directory: null | string,
					version: typeof Kernel.Tool.Wwise.SoundBank.Version.Value,
				): void {
					let version_c = Kernel.Tool.Wwise.SoundBank.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Wwise.SoundBank.Definition.SoundBank.default();
					Kernel.Tool.Wwise.SoundBank.Decode.process(data_stream, definition, Kernel.PathOptional.value(embedded_media_directory), version_c);
					if (definition_file !== null) {
						JSON.write_fs(definition_file, definition.get_json(version_c));
					}
					return;
				}

			}

		}

		export namespace Marmalade {

			export namespace DZip {

				export type VersionNumber = typeof Kernel.Tool.Marmalade.DZip.Version.Value.number;

				export const VersionNumberE = [0n] as VersionNumber[];

				export function pack_fs(
					data_file: string,
					definition_file: string,
					resource_directory: string,
					version: typeof Kernel.Tool.Marmalade.DZip.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.Marmalade.DZip.Version.value(version);
					let definition = Kernel.Tool.Marmalade.DZip.Definition.Package.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.Marmalade.DZip.Pack.process(data_stream, definition, Kernel.Path.value(resource_directory), version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					definition_file: null | string,
					resource_directory: null | string,
					version: typeof Kernel.Tool.Marmalade.DZip.Version.Value,
				): void {
					let version_c = Kernel.Tool.Marmalade.DZip.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Marmalade.DZip.Definition.Package.default();
					Kernel.Tool.Marmalade.DZip.Unpack.process(data_stream, definition, Kernel.PathOptional.value(resource_directory), version_c);
					if (definition_file !== null) {
						JSON.write_fs(definition_file, definition.get_json(version_c));
					}
					return;
				}

			}

		}

		export namespace PopCap {

			export namespace ZLib {

				export type VersionVariant64 = typeof Kernel.Tool.PopCap.ZLib.Version.Value.variant_64;

				export const VersionVariant64E = [false, true] as VersionVariant64[];

				export function compress_fs(
					raw_file: string,
					ripe_file: string,
					level: Data.Compression.Deflate.Level,
					window_bits: Data.Compression.Deflate.WindowBits,
					memory_level: Data.Compression.Deflate.MemoryLevel,
					strategy: Data.Compression.Deflate.Strategy,
					version: typeof Kernel.Tool.PopCap.ZLib.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.ZLib.Version.value(version);
					let raw = Storage.read_file(raw_file);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					let ripe_size_bound = Kernel.Size.default();
					Kernel.Tool.PopCap.ZLib.Compress.estimate(raw.size(), ripe_size_bound, Kernel.Size.value(window_bits), Kernel.Size.value(memory_level), version_c);
					let ripe = Kernel.ByteArray.allocate(ripe_size_bound);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					Kernel.Tool.PopCap.ZLib.Compress.process(raw_stream, ripe_stream, Kernel.Size.value(level), Kernel.Size.value(window_bits), Kernel.Size.value(memory_level), Kernel.Tool.Data.Compression.Deflate.Strategy.value(strategy), version_c);
					Storage.write_file(ripe_file, ripe_stream.stream_view());
					return;
				}

				export function uncompress_fs(
					ripe_file: string,
					raw_file: string,
					window_bits: Data.Compression.Deflate.WindowBits,
					version: typeof Kernel.Tool.PopCap.ZLib.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.ZLib.Version.value(version);
					let ripe = Storage.read_file(ripe_file);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					let raw_size = Kernel.Size.default();
					Kernel.Tool.PopCap.ZLib.Uncompress.estimate(ripe.view(), raw_size, version_c);
					let raw = Kernel.ByteArray.allocate(raw_size);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					Kernel.Tool.PopCap.ZLib.Uncompress.process(ripe_stream, raw_stream, Kernel.Size.value(window_bits), version_c);
					Storage.write_file(raw_file, raw_stream.stream_view());
					return;
				}

			}

			export namespace CryptData {

				export function encrypt_fs(
					plain_file: string,
					cipher_file: string,
					limit: bigint,
					key: string,
					version: typeof Kernel.Tool.PopCap.CryptData.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.CryptData.Version.value(version);
					let plain = Storage.read_file(plain_file);
					let plain_stream = Kernel.ByteStreamView.watch(plain.view());
					let cipher_size = Kernel.Size.default();
					Kernel.Tool.PopCap.CryptData.Encrypt.estimate(plain.size(), cipher_size, Kernel.Size.value(limit), version_c);
					let cipher = Kernel.ByteArray.allocate(cipher_size);
					let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
					Kernel.Tool.PopCap.CryptData.Encrypt.process(plain_stream, cipher_stream, Kernel.Size.value(limit), Kernel.String.value(key), version_c);
					Storage.write_file(cipher_file, cipher_stream.stream_view());
					return;
				}

				export function decrypt_fs(
					cipher_file: string,
					plain_file: string,
					limit: bigint,
					key: string,
					version: typeof Kernel.Tool.PopCap.CryptData.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.CryptData.Version.value(version);
					let cipher = Storage.read_file(cipher_file);
					let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
					let plain_size = Kernel.Size.default();
					Kernel.Tool.PopCap.CryptData.Decrypt.estimate(cipher.view(), plain_size, Kernel.Size.value(limit), version_c);
					let plain = Kernel.ByteArray.allocate(plain_size);
					let plain_stream = Kernel.ByteStreamView.watch(plain.view());
					Kernel.Tool.PopCap.CryptData.Decrypt.process(cipher_stream, plain_stream, Kernel.Size.value(limit), Kernel.String.value(key), version_c);
					Storage.write_file(plain_file, plain_stream.stream_view());
					return;
				}

			}

			export namespace ReflectionObjectNotation {

				export type VersionNumber = typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value.number;

				export const VersionNumberE = [1n] as VersionNumber[];

				export type VersionNativeStringEncodingUseUTF8 = typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value.native_string_encoding_use_utf8;

				export const VersionNativeStringEncodingUseUTF8E = [false, true] as VersionNativeStringEncodingUseUTF8[];

				export function encode_fs(
					data_file: string,
					definition_file: string,
					enable_string_index: boolean,
					enable_rtid: boolean,
					version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.ReflectionObjectNotation.Version.value(version);
					let definition = JSON.read_fs<Kernel.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>(definition_file);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.ReflectionObjectNotation.Encode.process(data_stream, definition, Kernel.Boolean.value(enable_string_index), Kernel.Boolean.value(enable_rtid), version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.ReflectionObjectNotation.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.JSON.Value.default<Kernel.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>();
					Kernel.Tool.PopCap.ReflectionObjectNotation.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition);
					return;
				}

				export function encode_cipher_fs(
					data_file: string,
					definition_file: string,
					enable_string_index: boolean,
					enable_rtid: boolean,
					version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
					key: null | string,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					if (key === null) {
						return encode_fs(data_file, definition_file, enable_string_index, enable_rtid, version, data_buffer);
					}
					let version_c = Kernel.Tool.PopCap.ReflectionObjectNotation.Version.value(version);
					let definition = JSON.read_fs<Kernel.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>(definition_file);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.ReflectionObjectNotation.Encode.process(data_stream, definition, Kernel.Boolean.value(enable_string_index), Kernel.Boolean.value(enable_rtid), version_c);
					let plain_stream = Kernel.ByteStreamView.watch(data_stream.stream_view());
					let cipher_size = Kernel.Size.default();
					Kernel.Tool.Miscellaneous.PvZ2CNCryptData.Encrypt.estimate(plain_stream.size(), cipher_size);
					let cipher = Kernel.ByteArray.allocate(cipher_size);
					let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
					Kernel.Tool.Miscellaneous.PvZ2CNCryptData.Encrypt.process(plain_stream, cipher_stream, Kernel.String.value(key));
					Storage.write_file(data_file, cipher_stream.stream_view());
					return;
				}

				export function decode_cipher_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value,
					key: null | string,
				): void {
					if (key === null) {
						return decode_fs(data_file, definition_file, version);
					}
					let version_c = Kernel.Tool.PopCap.ReflectionObjectNotation.Version.value(version);
					let cipher = Storage.read_file(data_file);
					let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
					let plain_size = Kernel.Size.default();
					Kernel.Tool.Miscellaneous.PvZ2CNCryptData.Decrypt.estimate(cipher.size(), plain_size);
					let plain = Kernel.ByteArray.allocate(plain_size);
					let plain_stream = Kernel.ByteStreamView.watch(plain.view());
					Kernel.Tool.Miscellaneous.PvZ2CNCryptData.Decrypt.process(cipher_stream, plain_stream, Kernel.String.value(key));
					let data_stream = Kernel.ByteStreamView.watch(plain_stream.stream_view());
					let definition = Kernel.JSON.Value.default<Kernel.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>();
					Kernel.Tool.PopCap.ReflectionObjectNotation.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition);
					return;
				}

			}

			export namespace UTexture {

				export type VersionCompressTextureData = typeof Kernel.Tool.PopCap.UTexture.Version.Value.compress_texture_data;

				export const VersionCompressTextureDataE = [false, true] as VersionCompressTextureData[];

				export type Format = [
					'rgba_8888_o',
					'rgba_4444',
					'rgba_5551',
					'rgb_565',
				][number];

				export const FormatE = [
					'rgba_8888_o',
					'rgba_4444',
					'rgba_5551',
					'rgb_565',
				] as Format[];

				export function encode_fs(
					data_file: string,
					image_file: string,
					format: Format,
					version: typeof Kernel.Tool.PopCap.UTexture.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.UTexture.Version.value(version);
					let image = KernelX.Image.File.PNG.read_fs_of(image_file);
					let image_view = image.view();
					let data_size_bound = Kernel.Size.default();
					Kernel.Tool.PopCap.UTexture.Encode.estimate(data_size_bound, image.size(), Kernel.Tool.Texture.Encoding.Format.value(format), version_c);
					let data = Kernel.ByteArray.allocate(data_size_bound);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					Kernel.Tool.PopCap.UTexture.Encode.process(data_stream, image_view, Kernel.Tool.Texture.Encoding.Format.value(format), version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					image_file: string,
					version: typeof Kernel.Tool.PopCap.UTexture.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.UTexture.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image_size = Kernel.Image.ImageSize.default();
					Kernel.Tool.PopCap.UTexture.Decode.estimate(data.view(), image_size, version_c);
					let image = Kernel.Image.Image.allocate(image_size);
					let image_view = image.view();
					Kernel.Tool.PopCap.UTexture.Decode.process(data_stream, image_view, version_c);
					KernelX.Image.File.PNG.write_fs(image_file, image_view);
					return;
				}

			}

			export namespace SexyTexture {

				export type VersionNumber = typeof Kernel.Tool.PopCap.SexyTexture.Version.Value.number;

				export const VersionNumberE = [0n] as VersionNumber[];

				export type Format = [
					'argb_8888',
					'argb_4444',
					'argb_1555',
					'rgb_565',
					'rgba_8888_o',
					'rgba_4444',
					'rgba_5551',
					// 'xrgb_8888',
					'la_88',
				][number];

				export const FormatE = [
					'argb_8888',
					'argb_4444',
					'argb_1555',
					'rgb_565',
					'rgba_8888_o',
					'rgba_4444',
					'rgba_5551',
					// 'xrgb_8888',
					'la_88',
				] as Format[];

				export function encode_fs(
					data_file: string,
					image_file: string,
					format: Format,
					compress_texture_data: boolean,
					version: typeof Kernel.Tool.PopCap.SexyTexture.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.SexyTexture.Version.value(version);
					let image = KernelX.Image.File.PNG.read_fs_of(image_file);
					let image_view = image.view();
					let data_size_bound = Kernel.Size.default();
					Kernel.Tool.PopCap.SexyTexture.Encode.estimate(data_size_bound, image.size(), Kernel.Tool.Texture.Encoding.Format.value(format), Kernel.Boolean.value(compress_texture_data), version_c);
					let data = Kernel.ByteArray.allocate(data_size_bound);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					Kernel.Tool.PopCap.SexyTexture.Encode.process(data_stream, image_view, Kernel.Tool.Texture.Encoding.Format.value(format), Kernel.Boolean.value(compress_texture_data), version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					image_file: string,
					version: typeof Kernel.Tool.PopCap.SexyTexture.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.SexyTexture.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image_size = Kernel.Image.ImageSize.default();
					Kernel.Tool.PopCap.SexyTexture.Decode.estimate(data.view(), image_size, version_c);
					let image = Kernel.Image.Image.allocate(image_size);
					let image_view = image.view();
					Kernel.Tool.PopCap.SexyTexture.Decode.process(data_stream, image_view, version_c);
					KernelX.Image.File.PNG.write_fs(image_file, image_view);
					return;
				}

			}

			export namespace Animation {

				export type VersionNumber = typeof Kernel.Tool.PopCap.Animation.Version.Value.number;

				export const VersionNumberE = [1n, 2n, 3n, 4n, 5n, 6n] as VersionNumber[];

				export function encode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.Animation.Version.value(version);
					let definition = Kernel.Tool.PopCap.Animation.Definition.Animation.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.Animation.Encode.process(data_stream, definition, version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.Animation.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.Animation.Definition.Animation.default();
					Kernel.Tool.PopCap.Animation.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace ReAnimation {

				export type VersionPlatform = typeof Kernel.Tool.PopCap.ReAnimation.Version.Value.platform;

				export const VersionPlatformE = ['desktop', 'mobile', 'television'] as VersionPlatform[];

				export type VersionVariant64 = typeof Kernel.Tool.PopCap.ReAnimation.Version.Value.variant_64;

				export const VersionVariant64E = [false, true] as VersionVariant64[];

				export function encode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.ReAnimation.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.ReAnimation.Version.value(version);
					let definition = Kernel.Tool.PopCap.ReAnimation.Definition.Animation.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.ReAnimation.Encode.process(data_stream, definition, version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.ReAnimation.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.ReAnimation.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.ReAnimation.Definition.Animation.default();
					Kernel.Tool.PopCap.ReAnimation.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace Particle {

				export type VersionPlatform = typeof Kernel.Tool.PopCap.Particle.Version.Value.platform;

				export const VersionPlatformE = ['desktop', 'mobile', 'television'] as VersionPlatform[];

				export type VersionVariant64 = typeof Kernel.Tool.PopCap.Particle.Version.Value.variant_64;

				export const VersionVariant64E = [false, true] as VersionVariant64[];

				export function encode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.Particle.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.Particle.Version.value(version);
					let definition = Kernel.Tool.PopCap.Particle.Definition.Particle.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.Particle.Encode.process(data_stream, definition, version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.Particle.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.Particle.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.Particle.Definition.Particle.default();
					Kernel.Tool.PopCap.Particle.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace Trail {

				export type VersionPlatform = typeof Kernel.Tool.PopCap.Trail.Version.Value.platform;

				export const VersionPlatformE = ['desktop', 'mobile', 'television'] as VersionPlatform[];

				export type VersionVariant64 = typeof Kernel.Tool.PopCap.Trail.Version.Value.variant_64;

				export const VersionVariant64E = [false, true] as VersionVariant64[];

				export function encode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.Trail.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.Trail.Version.value(version);
					let definition = Kernel.Tool.PopCap.Trail.Definition.Trail.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.Trail.Encode.process(data_stream, definition, version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.Trail.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.Trail.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.Trail.Definition.Trail.default();
					Kernel.Tool.PopCap.Trail.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace RenderEffect {

				export type VersionNumber = typeof Kernel.Tool.PopCap.RenderEffect.Version.Value.number;

				export const VersionNumberE = [1n] as VersionNumber[];

				export type VersionVariant = typeof Kernel.Tool.PopCap.RenderEffect.Version.Value.variant;

				export const VersionVariantE = [1n, 2n, 3n] as VersionVariant[];

				export function encode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.RenderEffect.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.RenderEffect.Version.value(version);
					let definition = Kernel.Tool.PopCap.RenderEffect.Definition.Effect.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.RenderEffect.Encode.process(data_stream, definition, version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.RenderEffect.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.RenderEffect.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.RenderEffect.Definition.Effect.default();
					Kernel.Tool.PopCap.RenderEffect.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace ParticleEffect {

				export type VersionNumber = typeof Kernel.Tool.PopCap.ParticleEffect.Version.Value.number;

				export const VersionNumberE = [1n] as VersionNumber[];

				export function encode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.ParticleEffect.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.ParticleEffect.Version.value(version);
					let definition = Kernel.Tool.PopCap.ParticleEffect.Definition.Effect.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.ParticleEffect.Encode.process(data_stream, definition, version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.ParticleEffect.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.ParticleEffect.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.ParticleEffect.Definition.Effect.default();
					Kernel.Tool.PopCap.ParticleEffect.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace CharacterFontWidget2 {

				export function encode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.CharacterFontWidget2.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.CharacterFontWidget2.Version.value(version);
					let definition = Kernel.Tool.PopCap.CharacterFontWidget2.Definition.FontWidget.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					data_stream.set_position(Kernel.Size.value(16n));
					Kernel.Tool.PopCap.CharacterFontWidget2.Encode.process(data_stream, definition, version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					definition_file: string,
					version: typeof Kernel.Tool.PopCap.CharacterFontWidget2.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.CharacterFontWidget2.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.CharacterFontWidget2.Definition.FontWidget.default();
					data_stream.set_position(Kernel.Size.value(16n));
					Kernel.Tool.PopCap.CharacterFontWidget2.Decode.process(data_stream, definition, version_c);
					JSON.write_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace Package {

				export type VersionNumber = typeof Kernel.Tool.PopCap.Package.Version.Value.number;

				export const VersionNumberE = [0n] as VersionNumber[];

				export type VersionCompressResourceData = typeof Kernel.Tool.PopCap.Package.Version.Value.compress_resource_data;

				export const VersionCompressResourceDataE = [false, true] as VersionCompressResourceData[];

				export function pack_fs(
					data_file: string,
					definition_file: string,
					resource_directory: string,
					version: typeof Kernel.Tool.PopCap.Package.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.Package.Version.value(version);
					let definition = Kernel.Tool.PopCap.Package.Definition.Package.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.Package.Pack.process(data_stream, definition, Kernel.Path.value(resource_directory), version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					definition_file: null | string,
					resource_directory: null | string,
					version: typeof Kernel.Tool.PopCap.Package.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.Package.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.Package.Definition.Package.default();
					Kernel.Tool.PopCap.Package.Unpack.process(data_stream, definition, Kernel.PathOptional.value(resource_directory), version_c);
					if (definition_file !== null) {
						JSON.write_fs(definition_file, definition.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamGroup {

				export type VersionNumber = typeof Kernel.Tool.PopCap.ResourceStreamGroup.Version.Value.number;

				export const VersionNumberE = [1n, 3n, 4n] as VersionNumber[];

				export function pack_fs(
					data_file: string,
					definition_file: string,
					resource_directory: string,
					version: typeof Kernel.Tool.PopCap.ResourceStreamGroup.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.ResourceStreamGroup.Version.value(version);
					let definition = Kernel.Tool.PopCap.ResourceStreamGroup.Definition.Package.json(JSON.read_fs(definition_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.ResourceStreamGroup.Pack.process(data_stream, definition, Kernel.Path.value(resource_directory), version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					definition_file: null | string,
					resource_directory: null | string,
					version: typeof Kernel.Tool.PopCap.ResourceStreamGroup.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.ResourceStreamGroup.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.ResourceStreamGroup.Definition.Package.default();
					Kernel.Tool.PopCap.ResourceStreamGroup.Unpack.process(data_stream, definition, Kernel.PathOptional.value(resource_directory), version_c);
					if (definition_file !== null) {
						JSON.write_fs(definition_file, definition.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamBundle {

				export type VersionNumber = typeof Kernel.Tool.PopCap.ResourceStreamBundle.Version.Value.number;

				export const VersionNumberE = [1n, 3n, 4n] as VersionNumber[];

				export type VersionaExtendedTextureInformationForPVZ2CN = typeof Kernel.Tool.PopCap.ResourceStreamBundle.Version.Value.extended_texture_information_for_pvz2_cn;

				export const VersionaExtendedTextureInformationForPVZ2CNE = [0n, 1n, 2n, 3n] as VersionaExtendedTextureInformationForPVZ2CN[];

				export function pack_fs(
					data_file: string,
					definition_file: string,
					manifest_file: string,
					resource_directory: string,
					packet_file: null | string,
					new_packet_file: null | string,
					version: typeof Kernel.Tool.PopCap.ResourceStreamBundle.Version.Value,
					data_buffer: Kernel.ByteListView | bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.ResourceStreamBundle.Version.value(version);
					let definition = Kernel.Tool.PopCap.ResourceStreamBundle.Definition.Package.json(JSON.read_fs(definition_file), version_c);
					let manifest = Kernel.Tool.PopCap.ResourceStreamBundle.Manifest.PackageOptional.json(JSON.read_fs(manifest_file), version_c);
					let data = is_bigint(data_buffer) ? Kernel.ByteArray.allocate(Kernel.Size.value(data_buffer)) : null;
					let data_stream = Kernel.ByteStreamView.watch(data_buffer instanceof Kernel.ByteListView ? data_buffer : data!.view());
					Kernel.Tool.PopCap.ResourceStreamBundle.Pack.process(data_stream, definition, manifest, Kernel.Path.value(resource_directory), Kernel.PathOptional.value(packet_file), Kernel.PathOptional.value(new_packet_file), version_c);
					Storage.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					definition_file: null | string,
					manifest_file: null | string,
					resource_directory: null | string,
					packet_file: null | string,
					version: typeof Kernel.Tool.PopCap.ResourceStreamBundle.Version.Value,
				): void {
					let version_c = Kernel.Tool.PopCap.ResourceStreamBundle.Version.value(version);
					let data = Storage.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.PopCap.ResourceStreamBundle.Definition.Package.default();
					let manifest = Kernel.Tool.PopCap.ResourceStreamBundle.Manifest.PackageOptional.default();
					Kernel.Tool.PopCap.ResourceStreamBundle.Unpack.process(data_stream, definition, manifest, Kernel.PathOptional.value(resource_directory), Kernel.PathOptional.value(packet_file), version_c);
					if (definition_file !== null) {
						JSON.write_fs(definition_file, definition.get_json(version_c));
					}
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamBundlePatch {

				export type VersionNumber = typeof Kernel.Tool.PopCap.ResourceStreamBundlePatch.Version.Value.number;

				export const VersionNumberE = [1n] as VersionNumber[];

				export function encode_fs(
					before_file: string,
					after_file: string,
					patch_file: string,
					use_raw_packet: boolean,
					version: typeof Kernel.Tool.PopCap.ResourceStreamBundlePatch.Version.Value,
					patch_size_bound: bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.ResourceStreamBundlePatch.Version.value(version);
					let before = Storage.read_file(before_file);
					let before_stream = Kernel.ByteStreamView.watch(before.view());
					let after = Storage.read_file(after_file);
					let after_stream = Kernel.ByteStreamView.watch(after.view());
					let patch = Kernel.ByteArray.allocate(Kernel.Size.value(patch_size_bound));
					let patch_stream = Kernel.ByteStreamView.watch(patch.view());
					Kernel.Tool.PopCap.ResourceStreamBundlePatch.Encode.process(before_stream, after_stream, patch_stream, Kernel.Boolean.value(use_raw_packet), version_c);
					Storage.write_file(patch_file, patch_stream.stream_view());
					return;
				}

				export function decode_fs(
					before_file: string,
					after_file: string,
					patch_file: string,
					use_raw_packet: boolean,
					version: typeof Kernel.Tool.PopCap.ResourceStreamBundlePatch.Version.Value,
					after_size_bound: bigint,
				): void {
					let version_c = Kernel.Tool.PopCap.ResourceStreamBundlePatch.Version.value(version);
					let before = Storage.read_file(before_file);
					let before_stream = Kernel.ByteStreamView.watch(before.view());
					let after = Kernel.ByteArray.allocate(Kernel.Size.value(after_size_bound));
					let after_stream = Kernel.ByteStreamView.watch(after.view());
					let patch = Storage.read_file(patch_file);
					let patch_stream = Kernel.ByteStreamView.watch(patch.view());
					Kernel.Tool.PopCap.ResourceStreamBundlePatch.Decode.process(before_stream, after_stream, patch_stream, Kernel.Boolean.value(use_raw_packet), version_c);
					Storage.write_file(after_file, after_stream.stream_view());
					return;
				}

			}

		}

		export namespace Miscellaneous {

			export namespace XboxTiledTexture {

				export function encode(
					data: Kernel.OByteStreamView,
					image: Kernel.Image.CImageView,
					format: Texture.Encoding.Format,
				): void {
					return Kernel.Tool.Miscellaneous.XboxTiledTexture.Encode.process(data, image, Kernel.Tool.Texture.Encoding.Format.value(format));
				}

				export function decode(
					data: Kernel.IByteStreamView,
					image: Kernel.Image.VImageView,
					format: Texture.Encoding.Format,
				): void {
					return Kernel.Tool.Miscellaneous.XboxTiledTexture.Decode.process(data, image, Kernel.Tool.Texture.Encoding.Format.value(format));
				}

			}

			export namespace PvZ2CNAlphaPaletteTexture {

				export type BitCount = 1 | 2 | 3 | 4;

				export function compute_bit_count(
					value_count: number,
				): BitCount {
					assert_test(0b1 < value_count && value_count <= 0b1 << 4, `invalue value count`);
					let bit_count: BitCount;
					if (value_count <= 0b10) {
						bit_count = 1;
					}
					else if (value_count <= 0b100) {
						bit_count = 2;
					}
					else if (value_count <= 0b1000) {
						bit_count = 3;
					}
					else {
						bit_count = 4;
					}
					return bit_count;
				}

				// ------------------------------------------------

				export function compute_data_size_with_palette(
					size: Image.ImageSize,
					index_count: number,
				): bigint {
					let bit_count = compute_bit_count(index_count);
					return 1n + BigInt(index_count === 2 ? 0 : index_count) + size[0] * size[1] * BigInt(bit_count) / 8n;
				}

				export function evaluate_palette(
					image: Kernel.Image.CImageView,
				): typeof Kernel.Image.ColorList.Value {
					let image_size = image.size().value;
					let data = Kernel.ByteArray.allocate(Kernel.Size.value(image_size[0] * image_size[1] * 8n / 8n));
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					Texture.Encoding.encode(data_stream, image, 'a_8');
					let alpha_count: Record<number, number> = {};
					for (let value of new Uint8Array(data_stream.stream_view().value)) {
						let alpha_4 = (value >> 4) & ~(~0 << 4);
						alpha_count[alpha_4] = not_undefined_or(alpha_count[alpha_4], 0) + 1;
					}
					let palette = Object.keys(alpha_count).map(BigInt);
					if (palette.length <= 2) {
						if (!palette.includes(0b0000n)) {
							palette.push(0b0000n);
						}
						if (!palette.includes(0b1111n)) {
							palette.push(0b1111n);
						}
						if (palette.length === 2) {
							palette = [0b0000n, 0b1111n];
						}
					}
					return palette;
				}

				// ------------------------------------------------

				export function encode_with_palette(
					data: Kernel.OByteStreamView,
					image: Kernel.Image.CImageView,
					palette: KernelX.Image.ColorList,
				): void {
					let palette_data = new ByteStreamView(data.view().value, Number(data.position().value));
					let bit_count = compute_bit_count(palette.length);
					if (bit_count === 1) {
						palette_data.u8(0n);
					}
					else {
						palette_data.u8(BigInt(palette.length));
						for (let value of palette) {
							palette_data.u8(value);
						}
					}
					data.set_position(Kernel.Size.value(data.position().value + BigInt(palette_data.p())));
					Kernel.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.Encode.process(data, image, Kernel.Image.ColorList.value(palette));
					return;
				}

				export function decode_with_palette(
					data: Kernel.IByteStreamView,
					image: Kernel.Image.VImageView,
				): void {
					let palette_data = new ByteStreamView(data.view().value, Number(data.position().value));
					let index_count = palette_data.u8();
					let palette: KernelX.Image.ColorList;
					if (index_count === 0n) {
						palette = [0b0000n, 0b1111n];
					}
					else {
						palette = [];
						for (let index = 0n; index < index_count; index++) {
							palette.push(palette_data.u8());
						}
					}
					data.set_position(Kernel.Size.value(data.position().value + BigInt(palette_data.p())));
					Kernel.Tool.Miscellaneous.PvZ2CNAlphaPaletteTexture.Decode.process(data, image, Kernel.Image.ColorList.value(palette));
					return;
				}

			}

			export namespace PvZ2CNCryptData {

				export function encrypt_fs(
					plain_file: string,
					cipher_file: string,
					key: string,
				): void {
					let plain = Storage.read_file(plain_file);
					let plain_stream = Kernel.ByteStreamView.watch(plain.view());
					let cipher_size = Kernel.Size.default();
					Kernel.Tool.Miscellaneous.PvZ2CNCryptData.Encrypt.estimate(plain.size(), cipher_size);
					let cipher = Kernel.ByteArray.allocate(cipher_size);
					let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
					Kernel.Tool.Miscellaneous.PvZ2CNCryptData.Encrypt.process(plain_stream, cipher_stream, Kernel.String.value(key));
					Storage.write_file(cipher_file, cipher_stream.stream_view());
					return;
				}

				export function decrypt_fs(
					cipher_file: string,
					plain_file: string,
					key: string,
				): void {
					let cipher = Storage.read_file(cipher_file);
					let cipher_stream = Kernel.ByteStreamView.watch(cipher.view());
					let plain_size = Kernel.Size.default();
					Kernel.Tool.Miscellaneous.PvZ2CNCryptData.Decrypt.estimate(cipher.size(), plain_size);
					let plain = Kernel.ByteArray.allocate(plain_size);
					let plain_stream = Kernel.ByteStreamView.watch(plain.view());
					Kernel.Tool.Miscellaneous.PvZ2CNCryptData.Decrypt.process(cipher_stream, plain_stream, Kernel.String.value(key));
					Storage.write_file(plain_file, plain_stream.stream_view());
					return;
				}

			}

		}

	}

	export namespace Miscellaneous {

		// ------------------------------------------------

		export function evaluate(
			script: string,
			name: string,
			is_module: boolean,
		): any {
			let script_c = Kernel.String.value(script);
			return Kernel.Miscellaneous.g_context.evaluate(Kernel.Miscellaneous.cast_String_to_CharacterListView(script_c), Kernel.String.value(name), Kernel.Boolean.value(is_module));
		}

		export function evaluate_fs(
			script_file: string,
			is_module: boolean,
		): any {
			let script = Storage.read_file(script_file);
			return Kernel.Miscellaneous.g_context.evaluate(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Kernel.String.value(script_file), Kernel.Boolean.value(is_module));
		}

		export function callback(
			argument: Array<string>,
		): Array<string> {
			return Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(argument)).value;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}