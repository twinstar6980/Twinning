namespace TwinStar.Script.CoreX {

	// ------------------------------------------------

	export const g_common_buffer = Core.ByteArray.default();

	// ------------------------------------------------

	export namespace JSON {

		// ------------------------------------------------

		export const g_format: {
			disable_trailing_comma: boolean;
			disable_array_wrap_line: boolean;
		} = {
			disable_trailing_comma: false,
			disable_array_wrap_line: false,
		};

		// ------------------------------------------------

		export function read<ConstraintT extends Core.JSON.JS_Value>(
			data: ArrayBuffer,
		): Core.JSON.Value<ConstraintT> {
			let data_stream = Core.CharacterStreamView.watch(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(Core.ByteListView.value(data)));
			let value = Core.JSON.Value.default<ConstraintT>();
			Core.Tool.Data.Serialization.JSON.Read.process_whole(data_stream, value);
			return value;
		}

		/** NOTE : result is a view of buffer */
		export function write<ConstraintT extends Core.JSON.JS_Value>(
			value: Core.JSON.Value<ConstraintT>,
			disable_trailing_comma: boolean = g_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_format.disable_array_wrap_line,
			data_buffer: Core.CharacterListView | bigint = Core.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): ArrayBuffer {
			let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
			let data_buffer_view = data_buffer instanceof Core.CharacterListView ? data_buffer : Core.Miscellaneous.cast_ByteListView_to_CharacterListView(data_buffer_if!.view());
			let data_stream = Core.CharacterStreamView.watch(data_buffer_view);
			Core.Tool.Data.Serialization.JSON.Write.process_whole(data_stream, value, Core.Boolean.value(disable_trailing_comma), Core.Boolean.value(disable_array_wrap_line));
			return Core.Miscellaneous.cast_CharacterListView_to_ByteListView(data_stream.stream_view()).value;
		}

		// ------------------------------------------------

		export function read_s<ConstraintT extends Core.JSON.JS_Value>(
			data: string,
		): Core.JSON.Value<ConstraintT> {
			let data_byte = Core.Miscellaneous.cast_moveable_String_to_ByteArray(Core.String.value(data));
			return read(data_byte.view().value);
		}

		export function write_s<ConstraintT extends Core.JSON.JS_Value>(
			value: Core.JSON.Value<ConstraintT>,
			disable_trailing_comma: boolean = g_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_format.disable_array_wrap_line,
			data_buffer: Core.CharacterListView | bigint = Core.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): string {
			let data = write(value, disable_trailing_comma, disable_array_wrap_line, data_buffer);
			return Core.Miscellaneous.cast_CharacterListView_to_JS_String(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(Core.ByteListView.value(data)));
		}

		// ------------------------------------------------

		export function read_fs<ConstraintT extends Core.JSON.JS_Value>(
			data_file: string,
		): Core.JSON.Value<ConstraintT> {
			let data = FileSystem.read_file(data_file);
			return read(data.view().value);
		}

		export function write_fs<ConstraintT extends Core.JSON.JS_Value>(
			data_file: string,
			value: Core.JSON.Value<ConstraintT>,
			disable_trailing_comma: boolean = g_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_format.disable_array_wrap_line,
			data_buffer: Core.CharacterListView | bigint = Core.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): void {
			let data = write(value, disable_trailing_comma, disable_array_wrap_line, data_buffer);
			FileSystem.write_file(data_file, data);
			return;
		}

		// ------------------------------------------------

		export function read_js<ConstraintT extends Core.JSON.JS_Value>(
			data: ArrayBuffer,
		): ConstraintT {
			return read<ConstraintT>(data).value;
		}

		/** NOTE : result is a view of buffer */
		export function write_js<ConstraintT extends Core.JSON.JS_Value>(
			value: ConstraintT,
			disable_trailing_comma: boolean = g_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_format.disable_array_wrap_line,
			data_buffer: Core.CharacterListView | bigint = Core.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): ArrayBuffer {
			return write(Core.JSON.Value.value<ConstraintT>(value), disable_trailing_comma, disable_array_wrap_line, data_buffer);
		}

		// ------------------------------------------------

		export function read_s_js<ConstraintT extends Core.JSON.JS_Value>(
			data: string,
		): ConstraintT {
			return read_s<ConstraintT>(data).value;
		}

		export function write_s_js<ConstraintT extends Core.JSON.JS_Value>(
			value: ConstraintT,
			disable_trailing_comma: boolean = g_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_format.disable_array_wrap_line,
			data_buffer: Core.CharacterListView | bigint = Core.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): string {
			return write_s(Core.JSON.Value.value<ConstraintT>(value), disable_trailing_comma, disable_array_wrap_line, data_buffer);
		}

		// ------------------------------------------------

		export function read_fs_js<ConstraintT extends Core.JSON.JS_Value>(
			data_file: string,
		): ConstraintT {
			return read_fs<ConstraintT>(data_file).value;
		}

		export function write_fs_js<ConstraintT extends Core.JSON.JS_Value>(
			data_file: string,
			value: ConstraintT,
			disable_trailing_comma: boolean = g_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_format.disable_array_wrap_line,
			data_buffer: Core.CharacterListView | bigint = Core.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
		): void {
			return write_fs(data_file, Core.JSON.Value.value<ConstraintT>(value), disable_trailing_comma, disable_array_wrap_line, data_buffer);
		}

		// ------------------------------------------------

	}

	export namespace XML {

		// ------------------------------------------------

		export function read_fs(
			data_file: string,
		): Core.XML.Node {
			let data_byte = FileSystem.read_file(data_file);
			let data = Core.Miscellaneous.cast_moveable_ByteArray_to_String(data_byte);
			let value = Core.XML.Node.default();
			Core.Tool.Data.Serialization.XML.Read.process_whole(data, value);
			return value;
		}

		export function write_fs(
			data_file: string,
			value: Core.XML.Node,
		): void {
			let data = Core.String.default();
			Core.Tool.Data.Serialization.XML.Write.process_whole(data, value);
			let data_byte = Core.Miscellaneous.cast_moveable_String_to_ByteArray(data);
			FileSystem.write_file(data_file, data_byte.view());
			return;
		}

		// ------------------------------------------------

		export function read_fs_js(
			data_file: string,
		): Core.XML.JS_Node {
			return read_fs(data_file).value;
		}

		export function write_fs_js(
			data_file: string,
			value: Core.XML.JS_Node,
		): void {
			return write_fs(data_file, Core.XML.Node.value(value));
		}

		// ------------------------------------------------

	}

	export namespace Image {

		// ------------------------------------------------

		export type ImageSize = typeof Core.Image.ImageSize.Value;

		export type ImagePosition = typeof Core.Image.ImagePosition.Value;

		// ------------------------------------------------

		export type Color = typeof Core.Image.Color.Value;

		export type ColorList = typeof Core.Image.ColorList.Value;

		export type Pixel = typeof Core.Image.Pixel.Value;

		// ------------------------------------------------

		export namespace File {

			// ------------------------------------------------

			export namespace PNG {

				// ------------------------------------------------

				export function size(
					data: Core.ByteListView,
				): ImageSize {
					let size = Core.Image.ImageSize.default();
					Core.Tool.Image.File.PNG.Read.compute_image_size(data, size);
					return size.value;
				}

				export function read(
					data: Core.ByteStreamView,
					image: Core.Image.VImageView,
				): void {
					return Core.Tool.Image.File.PNG.Read.process_image(data, image);
				}

				export function write(
					data: Core.ByteStreamView,
					image: Core.Image.CImageView,
				): void {
					return Core.Tool.Image.File.PNG.Write.process_image(data, image);
				}

				// ------------------------------------------------

				// NOTE : avoid use this function
				export function size_fs(
					file: string,
				): ImageSize {
					let data = FileSystem.read_file(file);
					let image_size = size(data.view());
					return image_size;
				}

				export function read_fs(
					file: string,
					image: Core.Image.VImageView,
				): void {
					let data = FileSystem.read_file(file);
					let data_stream = Core.ByteStreamView.watch(data.view());
					read(data_stream, image);
					return;
				}

				export function write_fs(
					file: string,
					image: Core.Image.CImageView,
					data_buffer: Core.ByteListView | bigint = g_common_buffer.view(),
				): void {
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let data_stream = Core.ByteStreamView.watch(data_buffer_view);
					write(data_stream, image);
					FileSystem.write_file(file, data_stream.stream_view());
					return;
				}

				export function read_fs_of(
					file: string,
				): Core.Image.Image {
					let data = FileSystem.read_file(file);
					let data_stream = Core.ByteStreamView.watch(data.view());
					let image_size = size(data.view());
					let image = Core.Image.Image.allocate(Core.Image.ImageSize.value(image_size));
					read(data_stream, image.view());
					return image;
				}

				// ------------------------------------------------

			}

			// ------------------------------------------------

		}

		// ------------------------------------------------

	}

	export namespace FileSystem {

		// ------------------------------------------------

		export function exist(
			target: string,
		): boolean {
			return Core.FileSystem.exist(Core.Path.value(target)).value;
		}

		export function exist_file(
			target: string,
		): boolean {
			return Core.FileSystem.exist_file(Core.Path.value(target)).value;
		}

		export function exist_directory(
			target: string,
		): boolean {
			return Core.FileSystem.exist_directory(Core.Path.value(target)).value;
		}

		// ------------------------------------------------

		export function copy(
			source: string,
			destination: string,
		): void {
			return Core.FileSystem.copy(Core.Path.value(source), Core.Path.value(destination));
		}

		export function rename(
			source: string,
			destination: string,
		): void {
			return Core.FileSystem.rename(Core.Path.value(source), Core.Path.value(destination));
		}

		export function remove(
			source: string,
		): void {
			return Core.FileSystem.remove(Core.Path.value(source));
		}

		// ------------------------------------------------

		export function create_link(
			target: string,
			object: string,
			is_directory: boolean,
		): void {
			return Core.FileSystem.create_link(Core.Path.value(target), Core.Path.value(object), Core.Boolean.value(is_directory));
		}

		export function parse_link(
			target: string,
		): string {
			return Core.FileSystem.parse_link(Core.Path.value(target)).value;
		}

		// ------------------------------------------------

		export function create_hard_link(
			target: string,
			object: string,
		): void {
			return Core.FileSystem.create_hard_link(Core.Path.value(target), Core.Path.value(object));
		}

		// ------------------------------------------------

		export function create_file(
			target: string,
		): void {
			return Core.FileSystem.create_file(Core.Path.value(target));
		}

		// ------------------------------------------------

		export function size_file(
			target: string,
		): bigint {
			return Core.FileSystem.size_file(Core.Path.value(target)).value;
		}

		export function resize_file(
			target: string,
			size: bigint,
		): void {
			return Core.FileSystem.resize_file(Core.Path.value(target), Core.Size.value(size));
		}

		// ------------------------------------------------

		export function read_file(
			target: string,
		): Core.ByteArray {
			return Core.FileSystem.read_file(Core.Path.value(target));
		}

		export function write_file(
			target: string,
			data: Core.ByteListView | Core.ByteArray | ArrayBuffer,
		): void {
			let data_view: Core.ByteListView;
			if (data instanceof Core.ByteListView) {
				data_view = data;
			}
			if (data instanceof Core.ByteArray) {
				data_view = data.view();
			}
			if (data instanceof ArrayBuffer) {
				data_view = Core.ByteListView.value(data);
			}
			return Core.FileSystem.write_file(Core.Path.value(target), data_view!);
		}

		// ------------------------------------------------

		export function create_directory(
			path: string,
		): void {
			return Core.FileSystem.create_directory(Core.Path.value(path));
		}

		// ------------------------------------------------

		export function count(
			target: string,
			depth: null | bigint = null,
		): bigint {
			return Core.FileSystem.count(Core.Path.value(target), Core.SizeOptional.value(depth)).value;
		}

		export function count_file(
			target: string,
			depth: null | bigint = null,
		): bigint {
			return Core.FileSystem.count_file(Core.Path.value(target), Core.SizeOptional.value(depth)).value;
		}

		export function count_directory(
			target: string,
			depth: null | bigint = null,
		): bigint {
			return Core.FileSystem.count_directory(Core.Path.value(target), Core.SizeOptional.value(depth)).value;
		}

		// ------------------------------------------------

		export function list(
			target: string,
			depth: null | bigint = null,
		): Array<string> {
			return Core.FileSystem.list(Core.Path.value(target), Core.SizeOptional.value(depth)).value;
		}

		export function list_file(
			target: string,
			depth: null | bigint = null,
		): Array<string> {
			return Core.FileSystem.list_file(Core.Path.value(target), Core.SizeOptional.value(depth)).value;
		}

		export function list_directory(
			target: string,
			depth: null | bigint = null,
		): Array<string> {
			return Core.FileSystem.list_directory(Core.Path.value(target), Core.SizeOptional.value(depth)).value;
		}

		// ------------------------------------------------

		export function get_working_directory(
		): string {
			return Core.FileSystem.get_working_directory().value;
		}

		export function set_working_directory(
			target: string,
		): void {
			return Core.FileSystem.set_working_directory(Core.Path.value(target));
		}

		// ------------------------------------------------

		export function get_temporary_directory(
		): string {
			return Core.FileSystem.get_temporary_directory().value;
		}

		// ------------------------------------------------

	}

	export namespace Process {

		// ------------------------------------------------

		export function environment(
		): Array<string> {
			return Core.Process.environment().value;
		}

		export function exit(
			code: bigint,
		): void {
			return Core.Process.exit(Core.IntegerU32.value(code));
		}

		export function execute(
			program: string,
			argument: Array<string>,
			environment: Array<string>,
			input: null | string,
			output: null | string,
			error: null | string,
		): bigint {
			return Core.Process.execute(Core.Path.value(program), Core.StringList.value(argument), Core.StringList.value(environment), Core.PathOptional.value(input), Core.PathOptional.value(output), Core.PathOptional.value(error)).value;
		}

		export function system(
			command: string,
		): bigint {
			return Core.Process.system(Core.String.value(command)).value;
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
						let data = FileSystem.read_file(data_file);
						let value = Core.ByteArray.default();
						Core.Tool.Data.Hash.MD5.Hash.process_whole(data.view(), value);
						return integer_from_byte_le(value.value);
					}

				}

				export namespace FNV {

					export function hash_fs(
						data_file: string,
						mode: typeof Core.Tool.Data.Hash.FNV.Mode.Value,
						bit_count: typeof Core.Tool.Data.Hash.FNV.BitCount.Value,
					): bigint {
						let data = FileSystem.read_file(data_file);
						let value = Core.ByteArray.default();
						Core.Tool.Data.Hash.FNV.Hash.process_whole(data.view(), value, Core.Tool.Data.Hash.FNV.Mode.value(mode), Core.Tool.Data.Hash.FNV.BitCount.value(bit_count));
						return integer_from_byte_le(value.value);
					}

					export function hash_s(
						data_string: string,
						mode: typeof Core.Tool.Data.Hash.FNV.Mode.Value,
						bit_count: typeof Core.Tool.Data.Hash.FNV.BitCount.Value,
					): bigint {
						let data = Core.Miscellaneous.cast_moveable_String_to_ByteArray(Core.String.value(data_string));
						let value = Core.ByteArray.default();
						Core.Tool.Data.Hash.FNV.Hash.process_whole(data.view(), value, Core.Tool.Data.Hash.FNV.Mode.value(mode), Core.Tool.Data.Hash.FNV.BitCount.value(bit_count));
						return integer_from_byte_le(value.value);
					}

				}

			}

			export namespace Encoding {

				export namespace Base64 {

					export function encode_fs(
						raw_file: string,
						ripe_file: string,
					): void {
						let raw_data = FileSystem.read_file(raw_file);
						let ripe_size = Core.Size.default();
						Core.Tool.Data.Encoding.Base64.Encode.compute_size(raw_data.size(), ripe_size);
						let ripe_data = Core.ByteArray.allocate(ripe_size);
						let raw_stream = Core.ByteStreamView.watch(raw_data.view());
						let ripe_stream = Core.CharacterStreamView.watch(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(ripe_data.view()));
						Core.Tool.Data.Encoding.Base64.Encode.process_whole(raw_stream, ripe_stream);
						FileSystem.write_file(ripe_file, Core.Miscellaneous.cast_CharacterListView_to_ByteListView(ripe_stream.stream_view()));
						return;
					}

					export function decode_fs(
						ripe_file: string,
						raw_file: string,
					): void {
						let ripe_data = FileSystem.read_file(ripe_file);
						let ripe_stream = Core.CharacterStreamView.watch(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(ripe_data.view()));
						let raw_size = Core.Size.default();
						Core.Tool.Data.Encoding.Base64.Decode.compute_size(ripe_stream.view(), raw_size);
						let raw_data = Core.ByteArray.allocate(raw_size);
						let raw_stream = Core.ByteStreamView.watch(raw_data.view());
						Core.Tool.Data.Encoding.Base64.Decode.process_whole(ripe_stream, raw_stream);
						FileSystem.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

			}

			export namespace Encryption {

				export namespace XOR {

					export function encrypt_fs(
						plain_file: string,
						cipher_file: string,
						key: Array<bigint>,
					): void {
						let key_c = Core.ByteArray.allocate(Core.Size.value(BigInt(key.length)));
						new Uint8Array(key_c.view().value).set(key.map((e) => (Number(e))));
						let plain_data = FileSystem.read_file(plain_file);
						let cipher_data = Core.ByteArray.allocate(plain_data.size());
						let plain_stream = Core.ByteStreamView.watch(plain_data.view());
						let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
						Core.Tool.Data.Encryption.XOR.Encrypt.process_whole(plain_stream, cipher_stream, key_c.view());
						FileSystem.write_file(cipher_file, cipher_stream.stream_view());
						return;
					}

				}

				export namespace Rijndael {

					export const ModeE = ['ecb', 'cbc', 'cfb'] as const;

					export type Mode = typeof ModeE[number];

					export const BlockSizeE = [16n, 24n, 32n] as const;

					export type BlockSize = typeof BlockSizeE[number];

					export function encrypt_fs(
						plain_file: string,
						cipher_file: string,
						mode: Mode,
						block_size: BlockSize,
						key_size: BlockSize,
						key: string,
						iv: string,
					): void {
						let plain_data = FileSystem.read_file(plain_file);
						let cipher_data = Core.ByteArray.allocate(plain_data.size());
						let plain_stream = Core.ByteStreamView.watch(plain_data.view());
						let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
						Core.Tool.Data.Encryption.Rijndael.Encrypt.process_whole(plain_stream, cipher_stream, Core.Tool.Data.Encryption.Rijndael.Mode.value(mode), Core.Size.value(block_size), Core.Size.value(key_size), Core.String.value(key), Core.String.value(iv));
						FileSystem.write_file(cipher_file, cipher_stream.stream_view());
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
						let cipher_data = FileSystem.read_file(cipher_file);
						let plain_data = Core.ByteArray.allocate(cipher_data.size());
						let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
						let plain_stream = Core.ByteStreamView.watch(plain_data.view());
						Core.Tool.Data.Encryption.Rijndael.Decrypt.process_whole(cipher_stream, plain_stream, Core.Tool.Data.Encryption.Rijndael.Mode.value(mode), Core.Size.value(block_size), Core.Size.value(key_size), Core.String.value(key), Core.String.value(iv));
						FileSystem.write_file(plain_file, plain_stream.stream_view());
						return;
					}

				}

			}

			export namespace Compression {

				export namespace Deflate {

					export const LevelE = [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type Level = typeof LevelE[number];

					export const WindowBitsE = [8n, 9n, 10n, 11n, 12n, 13n, 14n, 15n] as const;

					export type WindowBits = typeof WindowBitsE[number];

					export const MemoryLevelE = [1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type MemoryLevel = typeof MemoryLevelE[number];

					export const StrategyE = ['default_mode', 'filtered', 'huffman_only', 'rle', 'fixed'] as const;

					export type Strategy = typeof StrategyE[number];

					export const WrapperTypeE = ['none', 'zlib', 'gzip'] as const;

					export type WrapperType = typeof WrapperTypeE[number];

					export function compress_fs(
						raw_file: string,
						ripe_file: string,
						level: Level,
						window_bits: WindowBits,
						memory_level: MemoryLevel,
						strategy: Strategy,
						wrapper: WrapperType,
					): void {
						let raw_data = FileSystem.read_file(raw_file);
						let ripe_size_bound = Core.Size.default();
						Core.Tool.Data.Compression.Deflate.Compress.compute_size_bound(raw_data.size(), ripe_size_bound, Core.Size.value(window_bits), Core.Size.value(memory_level), Core.Tool.Data.Compression.Deflate.Wrapper.value(wrapper));
						let ripe_data = Core.ByteArray.allocate(ripe_size_bound);
						let raw_stream = Core.ByteStreamView.watch(raw_data.view());
						let ripe_stream = Core.ByteStreamView.watch(ripe_data.view());
						Core.Tool.Data.Compression.Deflate.Compress.process_whole(raw_stream, ripe_stream, Core.Size.value(level), Core.Size.value(window_bits), Core.Size.value(memory_level), Core.Tool.Data.Compression.Deflate.Strategy.value(strategy), Core.Tool.Data.Compression.Deflate.Wrapper.value(wrapper));
						FileSystem.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						ripe_file: string,
						raw_file: string,
						window_bits: WindowBits,
						wrapper: WrapperType,
						raw_data_buffer: Core.ByteListView | bigint,
					): void {
						let raw_data_buffer_if = typeof raw_data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(raw_data_buffer)) : null;
						let raw_data_buffer_view = raw_data_buffer instanceof Core.ByteListView ? raw_data_buffer : raw_data_buffer_if!.view();
						let ripe_data = FileSystem.read_file(ripe_file);
						let ripe_stream = Core.ByteStreamView.watch(ripe_data.view());
						let raw_stream = Core.ByteStreamView.watch(raw_data_buffer_view);
						Core.Tool.Data.Compression.Deflate.Uncompress.process_whole(ripe_stream, raw_stream, Core.Size.value(window_bits), Core.Tool.Data.Compression.Deflate.Wrapper.value(wrapper));
						FileSystem.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

				export namespace BZip2 {

					export const BlockSizeE = [1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type BlockSize = typeof BlockSizeE[number];

					export function compress_fs(
						raw_file: string,
						ripe_file: string,
						block_size: BlockSize,
					): void {
						let raw_data = FileSystem.read_file(raw_file);
						let ripe_size_bound = Core.Size.value(raw_data.size().value + 128n); // TODO
						let ripe_data = Core.ByteArray.allocate(ripe_size_bound);
						let raw_stream = Core.ByteStreamView.watch(raw_data.view());
						let ripe_stream = Core.ByteStreamView.watch(ripe_data.view());
						Core.Tool.Data.Compression.BZip2.Compress.process_whole(raw_stream, ripe_stream, Core.Size.value(block_size), Core.Size.value(0n));
						FileSystem.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						ripe_file: string,
						raw_file: string,
						raw_data_buffer: Core.ByteListView | bigint,
					): void {
						let raw_data_buffer_if = typeof raw_data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(raw_data_buffer)) : null;
						let raw_data_buffer_view = raw_data_buffer instanceof Core.ByteListView ? raw_data_buffer : raw_data_buffer_if!.view();
						let ripe_data = FileSystem.read_file(ripe_file);
						let ripe_stream = Core.ByteStreamView.watch(ripe_data.view());
						let raw_stream = Core.ByteStreamView.watch(raw_data_buffer_view);
						Core.Tool.Data.Compression.BZip2.Uncompress.process_whole(ripe_stream, raw_stream, Core.Boolean.value(false));
						FileSystem.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

				export namespace Lzma {

					export const LevelE = [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type Level = typeof LevelE[number];

					export function compress_fs(
						raw_file: string,
						ripe_file: string,
						level: Level,
					): void {
						let raw_data = FileSystem.read_file(raw_file);
						let ripe_size_bound = Core.Size.value(raw_data.size().value + 128n); // TODO
						let ripe_data = Core.ByteArray.allocate(ripe_size_bound);
						let raw_stream = Core.ByteStreamView.watch(raw_data.view());
						let ripe_stream = Core.ByteStreamView.watch(ripe_data.view());
						Core.Tool.Data.Compression.Lzma.Compress.process_whole(raw_stream, ripe_stream, Core.Size.value(level));
						FileSystem.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						ripe_file: string,
						raw_file: string,
						raw_data_buffer: Core.ByteListView | bigint,
					): void {
						let raw_data_buffer_if = typeof raw_data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(raw_data_buffer)) : null;
						let raw_data_buffer_view = raw_data_buffer instanceof Core.ByteListView ? raw_data_buffer : raw_data_buffer_if!.view();
						let ripe_data = FileSystem.read_file(ripe_file);
						let ripe_stream = Core.ByteStreamView.watch(ripe_data.view());
						let raw_stream = Core.ByteStreamView.watch(raw_data_buffer_view);
						Core.Tool.Data.Compression.Lzma.Uncompress.process_whole(ripe_stream, raw_stream);
						FileSystem.write_file(raw_file, raw_stream.stream_view());
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
						let before_data = FileSystem.read_file(before_file);
						let after_data = FileSystem.read_file(after_file);
						let patch_data = Core.ByteArray.allocate(Core.Size.value(patch_size_bound));
						let before_stream = Core.ByteStreamView.watch(before_data.view());
						let after_stream = Core.ByteStreamView.watch(after_data.view());
						let patch_stream = Core.ByteStreamView.watch(patch_data.view());
						Core.Tool.Data.Differentiation.VCDiff.Encode.process_whole(before_stream, after_stream, patch_stream, Core.Boolean.value(interleaved));
						FileSystem.write_file(patch_file, patch_stream.stream_view());
						return;
					}

					export function decode_fs(
						before_file: string,
						after_file: string,
						patch_file: string,
						maximum_window_size: bigint,
						after_size_bound: bigint,
					): void {
						let before_data = FileSystem.read_file(before_file);
						let after_data = Core.ByteArray.allocate(Core.Size.value(after_size_bound));
						let patch_data = FileSystem.read_file(patch_file);
						let before_stream = Core.ByteStreamView.watch(before_data.view());
						let after_stream = Core.ByteStreamView.watch(after_data.view());
						let patch_stream = Core.ByteStreamView.watch(patch_data.view());
						Core.Tool.Data.Differentiation.VCDiff.Decode.process_whole(before_stream, after_stream, patch_stream, Core.Size.value(maximum_window_size));
						FileSystem.write_file(after_file, after_stream.stream_view());
						return;
					}

				}

			}

		}

		export namespace Image {

			export type ImageSize = CoreX.Image.ImageSize;

			export type ImagePosition = CoreX.Image.ImagePosition;

			export type Color = CoreX.Image.Color;

			export type ColorList = CoreX.Image.ColorList;

			export type Pixel = CoreX.Image.Pixel;

			// ------------------------------------------------

			export namespace Transformation {

				export function flip(
					target: Core.Image.VImageView,
					horizontal: boolean,
					vertical: boolean,
				): void {
					return Core.Tool.Image.Transformation.Flip.process_image(target, Core.Boolean.value(horizontal), Core.Boolean.value(vertical));
				}

				export function scale(
					source: Core.Image.CImageView,
					destination: Core.Image.VImageView,
				): void {
					return Core.Tool.Image.Transformation.Scale.process_image(source, destination);
				}

				// ------------------------------------------------

				export function flip_fs(
					source_file: string,
					destination_file: string,
					horizontal: boolean,
					vertical: boolean,
				): void {
					let target = CoreX.Image.File.PNG.read_fs_of(source_file);
					let target_view = target.view();
					flip(target_view, horizontal, vertical);
					CoreX.Image.File.PNG.write_fs(destination_file, target_view);
					return;
				}

				export function scale_fs(
					source_file: string,
					destination_file: string,
					size: ImageSize,
				): void {
					let source = CoreX.Image.File.PNG.read_fs_of(source_file);
					let destination = Core.Image.Image.allocate(Core.Image.ImageSize.value(size));
					let source_view = source.view();
					let destination_view = destination.view();
					scale(source_view, destination_view);
					CoreX.Image.File.PNG.write_fs(destination_file, destination_view);
					return;
				}

				export function scale_rate_fs(
					source_file: string,
					destination_file: string,
					size_rate: number,
				): void {
					let source = CoreX.Image.File.PNG.read_fs_of(source_file);
					let destination = Core.Image.Image.allocate(Core.Image.ImageSize.value([BigInt(Math.max(1, Math.round(Number(source.size().value[0]) * size_rate))), BigInt(Math.max(1, Math.round(Number(source.size().value[1]) * size_rate)))]));
					let source_view = source.view();
					let destination_view = destination.view();
					scale(source_view, destination_view);
					CoreX.Image.File.PNG.write_fs(destination_file, destination_view);
					return;
				}

			}

			export namespace Texture {

				// ------------------------------------------------

				export const FormatE = [
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

				export type Format = typeof FormatE[number];

				export const CompressionE = [
					'rgb_etc1',
					'rgb_etc2',
					'rgba_etc2',
					'rgb_pvrtc4',
					'rgba_pvrtc4',
				] as const;

				export type Compression = typeof CompressionE[number];

				export const CompositeFormatE = [
					...FormatE,
					...CompressionE,
				] as const;

				export type CompositeFormat = typeof CompositeFormatE[number];

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

				export function compute_data_size_n(
					size: Image.ImageSize,
					format: Array<CompositeFormat>,
				): bigint {
					let data_size = 0n;
					for (let e of format) {
						data_size += compute_data_size(size, e);
					}
					return data_size;
				}

				// ------------------------------------------------

				export function encode(
					data: Core.OByteStreamView,
					image: Core.Image.CImageView,
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
							Core.Tool.Image.Texture.Encode.process_image(data, image, Core.Tool.Image.Texture.Format.value(format));
							break;
						}
						case 'rgb_etc1': {
							Core.Tool.Image.Texture.Compression.ETC1.Compress.process_image(data, image);
							break;
						}
						case 'rgb_etc2': {
							Core.Tool.Image.Texture.Compression.ETC2.Compress.process_image(data, image, Core.Boolean.value(false));
							break;
						}
						case 'rgba_etc2': {
							Core.Tool.Image.Texture.Compression.ETC2.Compress.process_image(data, image, Core.Boolean.value(true));
							break;
						}
						case 'rgb_pvrtc4': {
							Core.Tool.Image.Texture.Compression.PVRTC4.Compress.process_image(data, image, Core.Boolean.value(false));
							break;
						}
						case 'rgba_pvrtc4': {
							Core.Tool.Image.Texture.Compression.PVRTC4.Compress.process_image(data, image, Core.Boolean.value(true));
							break;
						}
					}
					return;
				}

				export function decode(
					data: Core.IByteStreamView,
					image: Core.Image.VImageView,
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
							Core.Tool.Image.Texture.Decode.process_image(data, image, Core.Tool.Image.Texture.Format.value(format));
							break;
						}
						case 'rgb_etc1': {
							Core.Tool.Image.Texture.Compression.ETC1.Uncompress.process_image(data, image);
							break;
						}
						case 'rgb_etc2': {
							Core.Tool.Image.Texture.Compression.ETC2.Uncompress.process_image(data, image, Core.Boolean.value(false));
							break;
						}
						case 'rgba_etc2': {
							Core.Tool.Image.Texture.Compression.ETC2.Uncompress.process_image(data, image, Core.Boolean.value(true));
							break;
						}
						case 'rgb_pvrtc4': {
							Core.Tool.Image.Texture.Compression.PVRTC4.Uncompress.process_image(data, image, Core.Boolean.value(false));
							break;
						}
						case 'rgba_pvrtc4': {
							Core.Tool.Image.Texture.Compression.PVRTC4.Uncompress.process_image(data, image, Core.Boolean.value(true));
							break;
						}
					}
					return;
				}

				// ------------------------------------------------

				export function encode_n(
					data: Core.OByteStreamView,
					image: Core.Image.CImageView,
					format: Array<CompositeFormat>,
				): void {
					for (let e of format) {
						encode(data, image, e);
					}
					return;
				}

				export function decode_n(
					data: Core.IByteStreamView,
					image: Core.Image.VImageView,
					format: Array<CompositeFormat>,
				): void {
					for (let e of format) {
						decode(data, image, e);
					}
					return;
				}

				// ------------------------------------------------

			}

		}

		export namespace Wwise {

			export namespace Media {

				export function decode_fs(
					ripe_file: string,
					raw_file: string,
					ffmpeg_program_file: string,
					ww2ogg_program_file: string,
					ww2ogg_code_book_file: string,
					temporary_directory: string,
					version: typeof Core.Tool.Wwise.Media.Version.Value,
				): void {
					let version_c = Core.Tool.Wwise.Media.Version.value(version);
					let ripe_data = FileSystem.read_file(ripe_file);
					let raw_data = Core.ByteArray.default();
					Core.Tool.Wwise.Media.Decode.process_media(ripe_data.view(), raw_data, Core.Path.value(ffmpeg_program_file), Core.Path.value(ww2ogg_program_file), Core.Path.value(ww2ogg_code_book_file), Core.Path.value(temporary_directory), version_c);
					FileSystem.write_file(raw_file, raw_data.view());
					return;
				}

			}

			export namespace SoundBank {

				// TODO
				export function detect_version(
					data: ArrayBuffer,
				): typeof Core.Tool.Wwise.SoundBank.Version.Value {
					let version: typeof Core.Tool.Wwise.SoundBank.Version.Value = {
						number: undefined!,
					};
					let view = new DataView(data);
					version.number = BigInt(view.getUint32(0x0)) as any;
					assert_test([88n, 112n, 140n].includes(version.number));
					return version;
				}

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					embedded_audio_directory: string,
					version: typeof Core.Tool.Wwise.SoundBank.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.Wwise.SoundBank.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.Wwise.SoundBank.Manifest.SoundBank.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.Wwise.SoundBank.Encode.process_sound_bank(stream, manifest, Core.Path.value(embedded_audio_directory), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: null | string,
					embedded_audio_directory: null | string,
					version: typeof Core.Tool.Wwise.SoundBank.Version.Value,
				): void {
					let version_c = Core.Tool.Wwise.SoundBank.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.Wwise.SoundBank.Manifest.SoundBank.default();
					Core.Tool.Wwise.SoundBank.Decode.process_sound_bank(stream, manifest, Core.PathOptional.value(embedded_audio_directory), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.get_json(version_c));
					}
					return;
				}

			}

		}

		export namespace Marmalade {

			export namespace DZip {

				export function pack_fs(
					data_file: string,
					manifest_file: string,
					resource_directory: string,
					version: typeof Core.Tool.Marmalade.DZip.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.Marmalade.DZip.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.Marmalade.DZip.Manifest.Package.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.Marmalade.DZip.Pack.process_package(stream, manifest, Core.Path.value(resource_directory), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					resource_directory: null | string,
					version: typeof Core.Tool.Marmalade.DZip.Version.Value,
				): void {
					let version_c = Core.Tool.Marmalade.DZip.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.Marmalade.DZip.Manifest.Package.default();
					Core.Tool.Marmalade.DZip.Unpack.process_package(stream, manifest, Core.PathOptional.value(resource_directory), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.get_json(version_c));
					}
					return;
				}

			}

		}

		export namespace PopCap {

			export namespace ZLib {

				export function compress_fs(
					raw_file: string,
					ripe_file: string,
					level: Data.Compression.Deflate.Level,
					window_bits: Data.Compression.Deflate.WindowBits,
					memory_level: Data.Compression.Deflate.MemoryLevel,
					strategy: Data.Compression.Deflate.Strategy,
					version: typeof Core.Tool.PopCap.ZLib.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.ZLib.Version.value(version);
					let raw_data = FileSystem.read_file(raw_file);
					let ripe_size_bound = Core.Size.default();
					Core.Tool.PopCap.ZLib.Compress.compute_size_bound(raw_data.size(), ripe_size_bound, Core.Size.value(window_bits), Core.Size.value(memory_level), version_c);
					let ripe_data = Core.ByteArray.allocate(ripe_size_bound);
					let raw_stream = Core.ByteStreamView.watch(raw_data.view());
					let ripe_stream = Core.ByteStreamView.watch(ripe_data.view());
					Core.Tool.PopCap.ZLib.Compress.process_whole(raw_stream, ripe_stream, Core.Size.value(level), Core.Size.value(window_bits), Core.Size.value(memory_level), Core.Tool.Data.Compression.Deflate.Strategy.value(strategy), version_c);
					FileSystem.write_file(ripe_file, ripe_stream.stream_view());
					return;
				}

				export function uncompress_fs(
					ripe_file: string,
					raw_file: string,
					window_bits: Data.Compression.Deflate.WindowBits,
					version: typeof Core.Tool.PopCap.ZLib.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.ZLib.Version.value(version);
					let ripe_data = FileSystem.read_file(ripe_file);
					let raw_size = Core.Size.default();
					Core.Tool.PopCap.ZLib.Uncompress.compute_size(ripe_data.view(), raw_size, version_c);
					let raw_data = Core.ByteArray.allocate(raw_size);
					let ripe_stream = Core.ByteStreamView.watch(ripe_data.view());
					let raw_stream = Core.ByteStreamView.watch(raw_data.view());
					Core.Tool.PopCap.ZLib.Uncompress.process_whole(ripe_stream, raw_stream, Core.Size.value(window_bits), version_c);
					FileSystem.write_file(raw_file, raw_stream.stream_view());
					return;
				}

			}

			export namespace CryptData {

				export function encrypt_fs(
					plain_file: string,
					cipher_file: string,
					limit: bigint,
					key: string,
					version: typeof Core.Tool.PopCap.CryptData.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.CryptData.Version.value(version);
					let plain_data = FileSystem.read_file(plain_file);
					let cipher_size = Core.Size.default();
					Core.Tool.PopCap.CryptData.Encrypt.compute_size(plain_data.size(), cipher_size, Core.Size.value(limit), version_c);
					let cipher_data = Core.ByteArray.allocate(cipher_size);
					let plain_stream = Core.ByteStreamView.watch(plain_data.view());
					let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
					Core.Tool.PopCap.CryptData.Encrypt.process_whole(plain_stream, cipher_stream, Core.Size.value(limit), Core.String.value(key), version_c);
					FileSystem.write_file(cipher_file, cipher_stream.stream_view());
					return;
				}

				export function decrypt_fs(
					cipher_file: string,
					plain_file: string,
					limit: bigint,
					key: string,
					version: typeof Core.Tool.PopCap.CryptData.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.CryptData.Version.value(version);
					let cipher_data = FileSystem.read_file(cipher_file);
					let plain_size = Core.Size.default();
					Core.Tool.PopCap.CryptData.Decrypt.compute_size(cipher_data.view(), plain_size, Core.Size.value(limit), version_c);
					let plain_data = Core.ByteArray.allocate(plain_size);
					let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
					let plain_stream = Core.ByteStreamView.watch(plain_data.view());
					Core.Tool.PopCap.CryptData.Decrypt.process_whole(cipher_stream, plain_stream, Core.Size.value(limit), Core.String.value(key), version_c);
					FileSystem.write_file(plain_file, plain_stream.stream_view());
					return;
				}

			}

			export namespace ReflectionObjectNotation {

				export function encode_fs(
					data_file: string,
					value_file: string,
					enable_string_index: boolean,
					enable_rtid: boolean,
					version: typeof Core.Tool.PopCap.ReflectionObjectNotation.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.ReflectionObjectNotation.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let value = JSON.read_fs<Core.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>(value_file);
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					Core.Tool.PopCap.ReflectionObjectNotation.Encode.process_whole(stream, value, Core.Boolean.value(enable_string_index), Core.Boolean.value(enable_rtid), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					value_file: string,
					version: typeof Core.Tool.PopCap.ReflectionObjectNotation.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.ReflectionObjectNotation.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let value = Core.JSON.Value.default<Core.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>();
					Core.Tool.PopCap.ReflectionObjectNotation.Decode.process_whole(stream, value, version_c);
					JSON.write_fs(value_file, value);
					return;
				}

				export function encrypt_fs(
					plain_file: string,
					cipher_file: string,
					key: string,
				): void {
					let plain_data = FileSystem.read_file(plain_file);
					let cipher_size = Core.Size.default();
					Core.Tool.PopCap.ReflectionObjectNotation.Encrypt.compute_size(plain_data.size(), cipher_size);
					let cipher_data = Core.ByteArray.allocate(cipher_size);
					let plain_stream = Core.ByteStreamView.watch(plain_data.view());
					let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
					Core.Tool.PopCap.ReflectionObjectNotation.Encrypt.process_whole(plain_stream, cipher_stream, Core.String.value(key));
					FileSystem.write_file(cipher_file, cipher_stream.stream_view());
					return;
				}

				export function decrypt_fs(
					cipher_file: string,
					plain_file: string,
					key: string,
				): void {
					let cipher_data = FileSystem.read_file(cipher_file);
					let plain_size = Core.Size.default();
					Core.Tool.PopCap.ReflectionObjectNotation.Decrypt.compute_size(cipher_data.size(), plain_size);
					let plain_data = Core.ByteArray.allocate(plain_size);
					let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
					let plain_stream = Core.ByteStreamView.watch(plain_data.view());
					Core.Tool.PopCap.ReflectionObjectNotation.Decrypt.process_whole(cipher_stream, plain_stream, Core.String.value(key));
					FileSystem.write_file(plain_file, plain_stream.stream_view());
					return;
				}

				export function encode_then_encrypt_fs(
					data_file: string,
					value_file: string,
					enable_string_index: boolean,
					enable_rtid: boolean,
					version: typeof Core.Tool.PopCap.ReflectionObjectNotation.Version.Value,
					key: string,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.ReflectionObjectNotation.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let value = JSON.read_fs<Core.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>(value_file);
					let data_stream = Core.ByteStreamView.watch(data_buffer_view);
					Core.Tool.PopCap.ReflectionObjectNotation.Encode.process_whole(data_stream, value, Core.Boolean.value(enable_string_index), Core.Boolean.value(enable_rtid), version_c);
					let plain_stream = Core.ByteStreamView.watch(data_stream.stream_view());
					let cipher_size = Core.Size.default();
					Core.Tool.PopCap.ReflectionObjectNotation.Encrypt.compute_size(plain_stream.size(), cipher_size);
					let cipher_data = Core.ByteArray.allocate(cipher_size);
					let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
					Core.Tool.PopCap.ReflectionObjectNotation.Encrypt.process_whole(plain_stream, cipher_stream, Core.String.value(key));
					FileSystem.write_file(data_file, cipher_stream.stream_view());
					return;
				}

				export function decrypt_then_decode_fs(
					data_file: string,
					value_file: string,
					version: typeof Core.Tool.PopCap.ReflectionObjectNotation.Version.Value,
					key: string,
				): void {
					let version_c = Core.Tool.PopCap.ReflectionObjectNotation.Version.value(version);
					let cipher_data = FileSystem.read_file(data_file);
					let plain_size = Core.Size.default();
					Core.Tool.PopCap.ReflectionObjectNotation.Decrypt.compute_size(cipher_data.size(), plain_size);
					let plain_data = Core.ByteArray.allocate(plain_size);
					let cipher_stream = Core.ByteStreamView.watch(cipher_data.view());
					let plain_stream = Core.ByteStreamView.watch(plain_data.view());
					Core.Tool.PopCap.ReflectionObjectNotation.Decrypt.process_whole(cipher_stream, plain_stream, Core.String.value(key));
					let data_stream = Core.ByteStreamView.watch(plain_stream.stream_view());
					let value = Core.JSON.Value.default<Core.Tool.PopCap.ReflectionObjectNotation.JS_ValidValue>();
					Core.Tool.PopCap.ReflectionObjectNotation.Decode.process_whole(data_stream, value, version_c);
					JSON.write_fs(value_file, value);
					return;
				}

			}

			export namespace UTexture {

				export const FormatE = [
					'rgba_8888_o',
					'rgba_4444',
					'rgba_5551',
					'rgb_565',
				] as const;

				export type Format = typeof FormatE[number];

				// ------------------------------------------------

				export function encode_fs(
					data_file: string,
					image_file: string,
					format: Format,
					version: typeof Core.Tool.PopCap.UTexture.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.UTexture.Version.value(version);
					let image = CoreX.Image.File.PNG.read_fs_of(image_file);
					let image_view = image.view();
					let data_size_bound = Core.Size.default();
					Core.Tool.PopCap.UTexture.Encode.compute_data_size_bound(data_size_bound, image.size(), Core.Tool.Image.Texture.Format.value(format), version_c);
					let data = Core.ByteArray.allocate(data_size_bound);
					let stream = Core.ByteStreamView.watch(data.view());
					Core.Tool.PopCap.UTexture.Encode.process_image(stream, image_view, Core.Tool.Image.Texture.Format.value(format), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					image_file: string,
					version: typeof Core.Tool.PopCap.UTexture.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.UTexture.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let image_size = Core.Image.ImageSize.default();
					Core.Tool.PopCap.UTexture.Decode.compute_image_size(data.view(), image_size, version_c);
					let image = Core.Image.Image.allocate(image_size);
					let image_view = image.view();
					Core.Tool.PopCap.UTexture.Decode.process_image(stream, image_view, version_c);
					CoreX.Image.File.PNG.write_fs(image_file, image_view);
					return;
				}

			}

			export namespace SexyTexture {

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
				] as const;

				export type Format = typeof FormatE[number];

				// ------------------------------------------------

				export function encode_fs(
					data_file: string,
					image_file: string,
					format: Format,
					compress_texture_data: boolean,
					version: typeof Core.Tool.PopCap.SexyTexture.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.SexyTexture.Version.value(version);
					let image = CoreX.Image.File.PNG.read_fs_of(image_file);
					let image_view = image.view();
					let data_size_bound = Core.Size.default();
					Core.Tool.PopCap.SexyTexture.Encode.compute_data_size_bound(data_size_bound, image.size(), Core.Tool.Image.Texture.Format.value(format), Core.Boolean.value(compress_texture_data), version_c);
					let data = Core.ByteArray.allocate(data_size_bound);
					let stream = Core.ByteStreamView.watch(data.view());
					Core.Tool.PopCap.SexyTexture.Encode.process_image(stream, image_view, Core.Tool.Image.Texture.Format.value(format), Core.Boolean.value(compress_texture_data), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					image_file: string,
					version: typeof Core.Tool.PopCap.SexyTexture.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.SexyTexture.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let image_size = Core.Image.ImageSize.default();
					Core.Tool.PopCap.SexyTexture.Decode.compute_image_size(data.view(), image_size, version_c);
					let image = Core.Image.Image.allocate(image_size);
					let image_view = image.view();
					Core.Tool.PopCap.SexyTexture.Decode.process_image(stream, image_view, version_c);
					CoreX.Image.File.PNG.write_fs(image_file, image_view);
					return;
				}

			}

			export namespace Animation {

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.Animation.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.Animation.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.Animation.Manifest.Animation.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.Animation.Encode.process_animation(stream, manifest, version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.Animation.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.Animation.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.Animation.Manifest.Animation.default();
					Core.Tool.PopCap.Animation.Decode.process_animation(stream, manifest, version_c);
					JSON.write_fs(manifest_file, manifest.get_json(version_c));
					return;
				}

			}

			export namespace ReAnimation {

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.ReAnimation.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.ReAnimation.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.ReAnimation.Manifest.Animation.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.ReAnimation.Encode.process_animation(stream, manifest, version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.ReAnimation.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.ReAnimation.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.ReAnimation.Manifest.Animation.default();
					Core.Tool.PopCap.ReAnimation.Decode.process_animation(stream, manifest, version_c);
					JSON.write_fs(manifest_file, manifest.get_json(version_c));
					return;
				}

			}

			export namespace Particle {

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.Particle.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.Particle.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.Particle.Manifest.Particle.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.Particle.Encode.process_particle(stream, manifest, version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.Particle.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.Particle.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.Particle.Manifest.Particle.default();
					Core.Tool.PopCap.Particle.Decode.process_particle(stream, manifest, version_c);
					JSON.write_fs(manifest_file, manifest.get_json(version_c));
					return;
				}

			}

			export namespace Trail {

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.Trail.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.Trail.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.Trail.Manifest.Trail.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.Trail.Encode.process_trail(stream, manifest, version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.Trail.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.Trail.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.Trail.Manifest.Trail.default();
					Core.Tool.PopCap.Trail.Decode.process_trail(stream, manifest, version_c);
					JSON.write_fs(manifest_file, manifest.get_json(version_c));
					return;
				}

			}

			export namespace Effect {

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.Effect.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.Effect.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.Effect.Manifest.Effect.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.Effect.Encode.process_effect(stream, manifest, version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.Effect.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.Effect.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.Effect.Manifest.Effect.default();
					Core.Tool.PopCap.Effect.Decode.process_effect(stream, manifest, version_c);
					JSON.write_fs(manifest_file, manifest.get_json(version_c));
					return;
				}

			}

			export namespace CharacterFontWidget2 {

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.CharacterFontWidget2.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.CharacterFontWidget2.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.CharacterFontWidget2.Manifest.FontWidget.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.CharacterFontWidget2.Encode.process_font_widget(stream, manifest, version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: string,
					version: typeof Core.Tool.PopCap.CharacterFontWidget2.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.CharacterFontWidget2.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.CharacterFontWidget2.Manifest.FontWidget.default();
					Core.Tool.PopCap.CharacterFontWidget2.Decode.process_font_widget(stream, manifest, version_c);
					JSON.write_fs(manifest_file, manifest.get_json(version_c));
					return;
				}

			}

			export namespace Package {

				export function pack_fs(
					data_file: string,
					manifest_file: string,
					resource_directory: string,
					version: typeof Core.Tool.PopCap.Package.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.Package.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.Package.Manifest.Package.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.Package.Pack.process_package(stream, manifest, Core.Path.value(resource_directory), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					resource_directory: null | string,
					version: typeof Core.Tool.PopCap.Package.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.Package.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.Package.Manifest.Package.default();
					Core.Tool.PopCap.Package.Unpack.process_package(stream, manifest, Core.PathOptional.value(resource_directory), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamGroup {

				export function pack_fs(
					data_file: string,
					manifest_file: string,
					resource_directory: string,
					version: typeof Core.Tool.PopCap.ResourceStreamGroup.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.ResourceStreamGroup.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.ResourceStreamGroup.Manifest.Package.json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.ResourceStreamGroup.Pack.process_package(stream, manifest, Core.Path.value(resource_directory), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					resource_directory: null | string,
					version: typeof Core.Tool.PopCap.ResourceStreamGroup.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.ResourceStreamGroup.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.ResourceStreamGroup.Manifest.Package.default();
					Core.Tool.PopCap.ResourceStreamGroup.Unpack.process_package(stream, manifest, Core.PathOptional.value(resource_directory), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamBundle {

				export function pack_fs(
					data_file: string,
					manifest_file: string,
					description_file: string,
					resource_directory: string,
					packet_file: null | string,
					new_packet_file: null | string,
					version: typeof Core.Tool.PopCap.ResourceStreamBundle.Version.Value,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.ResourceStreamBundle.Version.value(version);
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.watch(data_buffer_view);
					let manifest = Core.Tool.PopCap.ResourceStreamBundle.Manifest.Package.json(JSON.read_fs(manifest_file), version_c);
					let description = Core.Tool.PopCap.ResourceStreamBundle.Description.PackageOptional.json(JSON.read_fs(description_file), version_c);
					Core.Tool.PopCap.ResourceStreamBundle.Pack.process_package(stream, manifest, description, Core.Path.value(resource_directory), Core.PathOptional.value(packet_file), Core.PathOptional.value(new_packet_file), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					description_file: null | string,
					resource_directory: null | string,
					packet_file: null | string,
					version: typeof Core.Tool.PopCap.ResourceStreamBundle.Version.Value,
				): void {
					let version_c = Core.Tool.PopCap.ResourceStreamBundle.Version.value(version);
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.watch(data.view());
					let manifest = Core.Tool.PopCap.ResourceStreamBundle.Manifest.Package.default();
					let description = Core.Tool.PopCap.ResourceStreamBundle.Description.PackageOptional.default();
					Core.Tool.PopCap.ResourceStreamBundle.Unpack.process_package(stream, manifest, description, Core.PathOptional.value(resource_directory), Core.PathOptional.value(packet_file), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.get_json(version_c));
					}
					if (description_file !== null) {
						JSON.write_fs(description_file, description.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamBundlePatch {

				export function encode_fs(
					before_file: string,
					after_file: string,
					patch_file: string,
					use_raw_packet: boolean,
					version: typeof Core.Tool.PopCap.ResourceStreamBundlePatch.Version.Value,
					patch_size_bound: bigint,
				): void {
					let version_c = Core.Tool.PopCap.ResourceStreamBundlePatch.Version.value(version);
					let before_data = FileSystem.read_file(before_file);
					let after_data = FileSystem.read_file(after_file);
					let patch_data = Core.ByteArray.allocate(Core.Size.value(patch_size_bound));
					let before_stream = Core.ByteStreamView.watch(before_data.view());
					let after_stream = Core.ByteStreamView.watch(after_data.view());
					let patch_stream = Core.ByteStreamView.watch(patch_data.view());
					Core.Tool.PopCap.ResourceStreamBundlePatch.Encode.process_whole(before_stream, after_stream, patch_stream, Core.Boolean.value(use_raw_packet), version_c);
					FileSystem.write_file(patch_file, patch_stream.stream_view());
					return;
				}

				export function decode_fs(
					before_file: string,
					after_file: string,
					patch_file: string,
					use_raw_packet: boolean,
					version: typeof Core.Tool.PopCap.ResourceStreamBundlePatch.Version.Value,
					after_size_bound: bigint,
				): void {
					let version_c = Core.Tool.PopCap.ResourceStreamBundlePatch.Version.value(version);
					let before_data = FileSystem.read_file(before_file);
					let after_data = Core.ByteArray.allocate(Core.Size.value(after_size_bound));
					let patch_data = FileSystem.read_file(patch_file);
					let before_stream = Core.ByteStreamView.watch(before_data.view());
					let after_stream = Core.ByteStreamView.watch(after_data.view());
					let patch_stream = Core.ByteStreamView.watch(patch_data.view());
					Core.Tool.PopCap.ResourceStreamBundlePatch.Decode.process_whole(before_stream, after_stream, patch_stream, Core.Boolean.value(use_raw_packet), version_c);
					FileSystem.write_file(after_file, after_stream.stream_view());
					return;
				}

			}

		}

		export namespace Miscellaneous {

			export namespace XboxTiledTexture {

				// ------------------------------------------------

				export function encode(
					data: Core.OByteStreamView,
					image: Core.Image.CImageView,
					format: Image.Texture.Format,
				): void {
					return Core.Tool.Miscellaneous.XboxTiledTexture.Encode.process_image(data, image, Core.Tool.Image.Texture.Format.value(format));
				}

				export function decode(
					data: Core.IByteStreamView,
					image: Core.Image.VImageView,
					format: Image.Texture.Format,
				): void {
					return Core.Tool.Miscellaneous.XboxTiledTexture.Decode.process_image(data, image, Core.Tool.Image.Texture.Format.value(format));
				}

				// ------------------------------------------------

			}

			export namespace PvZ2ChineseAndroidAlphaPaletteTexture {

				// ------------------------------------------------

				export type BitCount = 1 | 2 | 3 | 4;

				export function compute_bit_count(
					value_count: number,
				): BitCount {
					assert_test(0b1 < value_count && value_count <= 0b1 << 4, `invalue value count`);
					let bit_count: BitCount;
					if (value_count <= 0b10) {
						bit_count = 1;
					} else if (value_count <= 0b100) {
						bit_count = 2;
					} else if (value_count <= 0b1000) {
						bit_count = 3;
					} else {
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
					image: Core.Image.CImageView,
				): typeof Core.Image.ColorList.Value {
					let image_size = image.size().value;
					let image_data = Core.ByteArray.allocate(Core.Size.value(image_size[0] * image_size[1] * 8n / 8n));
					let image_stream = Core.ByteStreamView.watch(image_data.view());
					Image.Texture.encode(image_stream, image, 'a_8');
					let alpha_count: Record<number, number> = {};
					for (let e of new Uint8Array(image_stream.stream_view().value)) {
						let alpha_4 = (e >> 4) & ~(~0 << 4);
						alpha_count[alpha_4] = (alpha_count[alpha_4] || 0) + 1;
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
					data: Core.OByteStreamView,
					image: Core.Image.CImageView,
					palette: CoreX.Image.ColorList,
				): void {
					let palette_data = new ByteStreamView(data.view().value, Number(data.position().value));
					let bit_count = compute_bit_count(palette.length);
					if (bit_count === 1) {
						palette_data.u8(0n);
					} else {
						palette_data.u8(BigInt(palette.length));
						for (let e of palette) {
							palette_data.u8(e);
						}
					}
					data.set_position(Core.Size.value(data.position().value + BigInt(palette_data.p())));
					Core.Tool.Miscellaneous.PvZ2ChineseAndroidAlphaPaletteTexture.Encode.process_image(data, image, Core.Image.ColorList.value(palette));
					return;
				}

				export function decode_with_palette(
					data: Core.IByteStreamView,
					image: Core.Image.VImageView,
				): void {
					let palette_data = new ByteStreamView(data.view().value, Number(data.position().value));
					let index_count = palette_data.u8();
					let palette: CoreX.Image.ColorList;
					if (index_count === 0n) {
						palette = [0b0000n, 0b1111n];
					} else {
						palette = [];
						for (let i = 0n; i < index_count; ++i) {
							palette.push(palette_data.u8());
						}
					}
					data.set_position(Core.Size.value(data.position().value + BigInt(palette_data.p())));
					Core.Tool.Miscellaneous.PvZ2ChineseAndroidAlphaPaletteTexture.Decode.process_image(data, image, Core.Image.ColorList.value(palette));
					return;
				}

				// ------------------------------------------------

			}

		}

	}

	export namespace Miscellaneous {

		// ------------------------------------------------

		export function evaluate(
			script: string,
			name: string = '<!evaluate>',
		): any {
			let script_s = Core.String.value(script);
			return Core.Miscellaneous.g_context.evaluate(Core.Miscellaneous.cast_String_to_CharacterListView(script_s), Core.String.value(name));
		}

		export function evaluate_fs(
			script_file: string,
			name: string = script_file,
		): any {
			let script = FileSystem.read_file(script_file);
			return Core.Miscellaneous.g_context.evaluate(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Core.String.value(name));
		}

		export function callback(
			argument: Array<string>,
		): Array<string> {
			return Core.Miscellaneous.g_context.callback(Core.StringList.value(argument)).value;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}