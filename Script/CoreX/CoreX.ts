/** 对 Core 中部分功能的包装 */
namespace TwinKleS.CoreX {

	export namespace JSON {

		// ------------------------------------------------

		const g_write_buffer = Core.ByteArray.default();

		export function set_write_buffer_size(
			size: bigint,
		): void {
			g_write_buffer.allocate(Core.Size.value(size));
			return;
		}

		// ------------------------------------------------

		const g_write_format: {
			disable_trailing_comma: boolean;
			disable_array_wrap_line: boolean;
		} = {
			disable_trailing_comma: false,
			disable_array_wrap_line: false,
		};

		export function set_write_format(
			disable_trailing_comma: boolean,
			disable_array_wrap_line: boolean,
		): void {
			g_write_format.disable_trailing_comma = disable_trailing_comma;
			g_write_format.disable_array_wrap_line = disable_array_wrap_line;
			return;
		}

		// ------------------------------------------------

		export function read<ConstraintT extends Core.JSON.JS_Value>(
			data: ArrayBuffer,
		): Core.JSON.Value<ConstraintT> {
			let stream = Core.CharacterStreamView.look(Core.Misc.cast_ByteListView_to_CharacterListView(Core.ByteListView.value(data)));
			let value = Core.JSON.Value.default<ConstraintT>();
			Core.JSON.Read.process(stream, value);
			return value;
		}

		/** NOTE : result is a view of buffer */
		export function write<ConstraintT extends Core.JSON.JS_Value>(
			value: Core.JSON.Value<ConstraintT>,
			disable_trailing_comma: boolean = g_write_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_write_format.disable_array_wrap_line,
			buffer: Core.CharacterListView | bigint = Core.Misc.cast_ByteListView_to_CharacterListView(g_write_buffer.view()),
		): ArrayBuffer {
			let buffer_if = typeof buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(buffer)) : null;
			let buffer_view = buffer instanceof Core.CharacterListView ? buffer : Core.Misc.cast_ByteListView_to_CharacterListView(buffer_if!.view());
			let stream = Core.CharacterStreamView.look(buffer_view);
			Core.JSON.Write.process(stream, value, Core.Boolean.value(disable_trailing_comma), Core.Boolean.value(disable_array_wrap_line), Core.Size.value(0n));
			return Core.Misc.cast_CharacterListView_to_ByteListView(stream.stream_view()).value;
		}

		// ------------------------------------------------

		export function read_s<ConstraintT extends Core.JSON.JS_Value>(
			string: string,
		): Core.JSON.Value<ConstraintT> {
			let data = Core.Misc.cast_moveable_String_to_ByteArray(Core.String.value(string));
			return read(data.view().value);
		}

		export function write_s<ConstraintT extends Core.JSON.JS_Value>(
			value: Core.JSON.Value<ConstraintT>,
			disable_trailing_comma: boolean = g_write_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_write_format.disable_array_wrap_line,
			buffer: Core.CharacterListView | bigint = Core.Misc.cast_ByteListView_to_CharacterListView(g_write_buffer.view()),
		): string {
			let data = write(value, disable_trailing_comma, disable_array_wrap_line, buffer);
			return Core.Misc.cast_CharacterListView_to_JS_String(Core.Misc.cast_ByteListView_to_CharacterListView(Core.ByteListView.value(data)));
		}

		// ------------------------------------------------

		export function read_fs<ConstraintT extends Core.JSON.JS_Value>(
			file: string,
		): Core.JSON.Value<ConstraintT> {
			let data = FileSystem.read_file(file);
			return read(data.view().value);
		}

		export function write_fs<ConstraintT extends Core.JSON.JS_Value>(
			file: string,
			value: Core.JSON.Value<ConstraintT>,
			disable_trailing_comma: boolean = g_write_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_write_format.disable_array_wrap_line,
			buffer: Core.CharacterListView | bigint = Core.Misc.cast_ByteListView_to_CharacterListView(g_write_buffer.view()),
		): void {
			let data = write(value, disable_trailing_comma, disable_array_wrap_line, buffer);
			FileSystem.write_file(file, data);
			return;
		}

		// ------------------------------------------------

		export function read_js<ConstraintT extends Core.JSON.JS_Value>(
			data: ArrayBuffer,
		): ConstraintT {
			return read<ConstraintT>(data).value;
		}

		export function write_js<ConstraintT extends Core.JSON.JS_Value>(
			value: ConstraintT,
			disable_trailing_comma: boolean = g_write_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_write_format.disable_array_wrap_line,
			buffer: Core.CharacterListView | bigint = Core.Misc.cast_ByteListView_to_CharacterListView(g_write_buffer.view()),
		): ArrayBuffer {
			return write(Core.JSON.Value.value<ConstraintT>(value), disable_trailing_comma, disable_array_wrap_line, buffer);
		}

		// ------------------------------------------------

		export function read_s_js<ConstraintT extends Core.JSON.JS_Value>(
			string: string,
		): ConstraintT {
			return read_s<ConstraintT>(string).value;
		}

		export function write_s_js<ConstraintT extends Core.JSON.JS_Value>(
			value: ConstraintT,
			disable_trailing_comma: boolean = g_write_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_write_format.disable_array_wrap_line,
			buffer: Core.CharacterListView | bigint = Core.Misc.cast_ByteListView_to_CharacterListView(g_write_buffer.view()),
		): string {
			return write_s(Core.JSON.Value.value<ConstraintT>(value), disable_trailing_comma, disable_array_wrap_line, buffer);
		}

		// ------------------------------------------------

		export function read_fs_js<ConstraintT extends Core.JSON.JS_Value>(
			file: string,
		): ConstraintT {
			return read_fs<ConstraintT>(file).value;
		}

		export function write_fs_js<ConstraintT extends Core.JSON.JS_Value>(
			file: string,
			value: ConstraintT,
			disable_trailing_comma: boolean = g_write_format.disable_trailing_comma,
			disable_array_wrap_line: boolean = g_write_format.disable_array_wrap_line,
			buffer: Core.CharacterListView | bigint = Core.Misc.cast_ByteListView_to_CharacterListView(g_write_buffer.view()),
		): void {
			return write_fs(file, Core.JSON.Value.value<ConstraintT>(value), disable_trailing_comma, disable_array_wrap_line, buffer);
		}

		// ------------------------------------------------

	}

	export namespace XML {

		// ------------------------------------------------

		export function read_fs(
			file: string,
		): Core.XML.Node {
			let data = FileSystem.read_file(file);
			let string = Core.Misc.cast_ByteListView_to_CharacterListView(data.view());
			return Core.XML.Read.process(string);
		}

		export function write_fs(
			file: string,
			value: Core.XML.Node,
		): void {
			let string = Core.XML.Write.process(value);
			FileSystem.write_file(file, Core.Misc.cast_CharacterListView_to_ByteListView(Core.Misc.cast_String_to_CharacterListView(string)));
			return;
		}

		// ------------------------------------------------

		export function read_fs_js(
			file: string,
		): Core.XML.JS_Node {
			return read_fs(file).value;
		}

		export function write_fs_js(
			file: string,
			value: Core.XML.JS_Node,
		): void {
			return write_fs(file, Core.XML.Node.value(value));
		}

		// ------------------------------------------------

	}

	export namespace Image {

		// ------------------------------------------------

		export type ImageSize = Core.Image.JS_ImageSize;

		export type ImagePosition = Core.Image.JS_ImagePosition;

		// ------------------------------------------------

		export namespace File {

			export function png_size_file(
				path: string,
			): ImageSize {
				return Core.Image.File.PNG.size_file(Core.Path.value(path)).value;
			}

			export function png_read_file(
				path: string,
				image: Core.Image.VBitmapView,
			): void {
				return Core.Image.File.PNG.read_file(Core.Path.value(path), image);
			}

			export function png_write_file(
				path: string,
				image: Core.Image.CBitmapView,
			): void {
				return Core.Image.File.PNG.write_file(Core.Path.value(path), image);
			}

			export function png_read_file_of(
				path: string,
			): Core.Image.Bitmap {
				return Core.Image.File.PNG.read_file_of(Core.Path.value(path));
			}

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
			dest: string,
		): void {
			return Core.FileSystem.copy(Core.Path.value(source), Core.Path.value(dest));
		}

		export function rename(
			source: string,
			dest: string,
		): void {
			return Core.FileSystem.rename(Core.Path.value(source), Core.Path.value(dest));
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

	export namespace System {

		// ------------------------------------------------

		export function exit(
			code: bigint,
		): void {
			return Core.System.exit(Core.IntegerS32.value(code));
		}

		export function sleep(
			time: bigint,
		): void {
			return Core.System.sleep(Core.Size.value(time));
		}

		// ------------------------------------------------

		export function system(
			command: string,
		): bigint {
			return Core.System.system(Core.String.value(command)).value;
		}

		export function process(
			path: string,
			argument: Array<string>,
		): bigint {
			return Core.System.process(Core.Path.value(path), Core.StringList.value(argument)).value;
		}

		// ------------------------------------------------

	}

	export namespace Tool {

		export namespace Data {

			export namespace Hash {

				export namespace MD5 {

					export function hash_fs(
						file: string,
						use_upper_case_number: boolean = false,
					): string {
						let data = FileSystem.read_file(file);
						return Core.Tool.Data.Hash.MD5.Hash.process_to_string(data.view(), Core.Boolean.value(use_upper_case_number)).value;
					}

				}

			}

			export namespace Encode {

				export namespace Base64 {

					export function encode_fs(
						raw_file: string,
						ripe_file: string,
					): void {
						let raw_data = FileSystem.read_file(raw_file);
						let ripe_size = Core.Tool.Data.Encode.Base64.Encode.compute_size(raw_data.size());
						let ripe_data = Core.ByteArray.allocate(ripe_size);
						let raw_stream = Core.ByteStreamView.look(raw_data.view());
						let ripe_stream = Core.CharacterStreamView.look(Core.Misc.cast_ByteListView_to_CharacterListView(ripe_data.view()));
						Core.Tool.Data.Encode.Base64.Encode.process(raw_stream, ripe_stream);
						FileSystem.write_file(ripe_file, Core.Misc.cast_CharacterListView_to_ByteListView(ripe_stream.stream_view()));
						return;
					}

					export function decode_fs(
						ripe_file: string,
						raw_file: string,
					): void {
						let ripe_data = FileSystem.read_file(ripe_file);
						let ripe_stream = Core.CharacterStreamView.look(Core.Misc.cast_ByteListView_to_CharacterListView(ripe_data.view()));
						let raw_size = Core.Tool.Data.Encode.Base64.Decode.compute_size(ripe_stream.view());
						let raw_data = Core.ByteArray.allocate(raw_size);
						let raw_stream = Core.ByteStreamView.look(raw_data.view());
						Core.Tool.Data.Encode.Base64.Decode.process(ripe_stream, raw_stream);
						FileSystem.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

			}

			export namespace Encrypt {

				export namespace XOR {

					export namespace Crypt {

						export function crypt_fs(
							plain_file: string,
							cipher_file: string,
							key: bigint,
						): void {
							let plain_data = FileSystem.read_file(plain_file);
							let cipher_data = Core.ByteArray.allocate(plain_data.size());
							let plain_stream = Core.ByteStreamView.look(plain_data.view());
							let cipher_stream = Core.ByteStreamView.look(cipher_data.view());
							Core.Tool.Data.Encrypt.XOR.Crypt.process(plain_stream, cipher_stream, Core.Byte.value(key));
							FileSystem.write_file(cipher_file, cipher_stream.stream_view());
							return;
						}

					}

				}

			}

			export namespace Compress {

				export namespace Deflate {

					export type CompressionLevel = 0n | 1n | 2n | 3n | 4n | 5n | 6n | 7n | 8n | 9n;

					export type WindowBits = 8n | 9n | 10n | 11n | 12n | 13n | 14n | 15n;

					export type MemoryLevel = 1n | 2n | 3n | 4n | 5n | 6n | 7n | 8n | 9n;

					export type Strategy = Core.Tool.Data.Compress.Deflate.JS_Strategy;

					export type WrapperType = Core.Tool.Data.Compress.Deflate.JS_Wrapper;

					export function compress_fs(
						raw_file: string,
						ripe_file: string,
						level: CompressionLevel,
						window_bits: WindowBits,
						memory_level: MemoryLevel,
						strategy: Strategy,
						wrapper: WrapperType,
					): void {
						let raw_data = FileSystem.read_file(raw_file);
						let ripe_size_bound = Core.Tool.Data.Compress.Deflate.Compress.compute_size_bound(raw_data.size(), Core.Size.value(window_bits), Core.Size.value(memory_level), Core.Tool.Data.Compress.Deflate.Wrapper.value(wrapper));
						let ripe_data = Core.ByteArray.allocate(ripe_size_bound);
						let raw_stream = Core.ByteStreamView.look(raw_data.view());
						let ripe_stream = Core.ByteStreamView.look(ripe_data.view());
						Core.Tool.Data.Compress.Deflate.Compress.process(raw_stream, ripe_stream, Core.Size.value(level), Core.Size.value(window_bits), Core.Size.value(memory_level), Core.Tool.Data.Compress.Deflate.Strategy.value('default_mode'), Core.Tool.Data.Compress.Deflate.Wrapper.value(wrapper));
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
						let ripe_stream = Core.ByteStreamView.look(ripe_data.view());
						let raw_stream = Core.ByteStreamView.look(raw_data_buffer_view);
						Core.Tool.Data.Compress.Deflate.Uncompress.process(ripe_stream, raw_stream, Core.Size.value(window_bits), Core.Tool.Data.Compress.Deflate.Wrapper.value(wrapper));
						FileSystem.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

				export namespace BZip2 {

					export type BlockSize = 1n | 2n | 3n | 4n | 5n | 6n | 7n | 8n | 9n;

					export function compress_fs(
						raw_file: string,
						ripe_file: string,
						block_size: BlockSize,
					): void {
						let raw_data = FileSystem.read_file(raw_file);
						let ripe_size_bound = Core.Size.value(raw_data.size().value + 128n); // TODO
						let ripe_data = Core.ByteArray.allocate(ripe_size_bound);
						let raw_stream = Core.ByteStreamView.look(raw_data.view());
						let ripe_stream = Core.ByteStreamView.look(ripe_data.view());
						Core.Tool.Data.Compress.BZip2.Compress.process(raw_stream, ripe_stream, Core.Size.value(block_size), Core.Size.value(0n));
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
						let ripe_stream = Core.ByteStreamView.look(ripe_data.view());
						let raw_stream = Core.ByteStreamView.look(raw_data_buffer_view);
						Core.Tool.Data.Compress.BZip2.Uncompress.process(ripe_stream, raw_stream, Core.Boolean.value(false));
						FileSystem.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

			}

		}

		export namespace Texture {

			// ------------------------------------------------

			export const FormatE = [
				'a_8',
				'rgb_888',
				'rgba_8888',
				'rgb_565_l',
				'rgba_4444_l',
				'rgba_5551_l',
				'argb_4444_l',
				'argb_8888_l',
			] as const;

			export const CompressionE = [
				'rgb_etc1',
				'rgb_pvrtc4',
				'rgba_pvrtc4',
			] as const;

			export const TextureFormatE = [
				...FormatE,
				...CompressionE,
			] as const;

			export type Format = typeof FormatE[number];

			export type Compression = typeof CompressionE[number];

			export type CompositeFormat = typeof TextureFormatE[number];

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
					case 'rgb_888': {
						result = 24n;
						break;
					}
					case 'rgba_8888': {
						result = 32n;
						break;
					}
					case 'rgb_565_l': {
						result = 16n;
						break;
					}
					case 'rgba_4444_l': {
						result = 16n;
						break;
					}
					case 'rgba_5551_l': {
						result = 16n;
						break;
					}
					case 'argb_4444_l': {
						result = 16n;
						break;
					}
					case 'argb_8888_l': {
						result = 32n;
						break;
					}
					case 'rgb_etc1': {
						result = 4n;
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
				image: Core.Image.CBitmapView,
				format: CompositeFormat,
			): void {
				switch (format) {
					case 'a_8':
					case 'rgb_888':
					case 'rgba_8888':
					case 'rgb_565_l':
					case 'rgba_4444_l':
					case 'rgba_5551_l':
					case 'argb_4444_l':
					case 'argb_8888_l': {
						Core.Tool.Texture.Encode.process(data, image, Core.Tool.Texture.Format.value(format));
						break;
					}
					case 'rgb_etc1': {
						Core.Tool.Texture.Compress.ETC1.Compress.process(data, image);
						break;
					}
					case 'rgb_pvrtc4': {
						Core.Tool.Texture.Compress.PVRTC4.Compress.process(data, image, Core.Boolean.value(false));
						break;
					}
					case 'rgba_pvrtc4': {
						Core.Tool.Texture.Compress.PVRTC4.Compress.process(data, image, Core.Boolean.value(true));
						break;
					}
				}
				return;
			}

			export function decode(
				data: Core.IByteStreamView,
				image: Core.Image.VBitmapView,
				format: CompositeFormat,
			): void {
				switch (format) {
					case 'a_8':
					case 'rgb_888':
					case 'rgba_8888':
					case 'rgb_565_l':
					case 'rgba_4444_l':
					case 'rgba_5551_l':
					case 'argb_4444_l':
					case 'argb_8888_l': {
						Core.Tool.Texture.Decode.process(data, image, Core.Tool.Texture.Format.value(format));
						break;
					}
					case 'rgb_etc1': {
						Core.Tool.Texture.Compress.ETC1.Uncompress.process(data, image);
						break;
					}
					case 'rgb_pvrtc4': {
						Core.Tool.Texture.Compress.PVRTC4.Uncompress.process(data, image, Core.Boolean.value(false));
						break;
					}
					case 'rgba_pvrtc4': {
						Core.Tool.Texture.Compress.PVRTC4.Uncompress.process(data, image, Core.Boolean.value(true));
						break;
					}
				}
				return;
			}

			// ------------------------------------------------

			export function encode_n(
				data: Core.OByteStreamView,
				image: Core.Image.CBitmapView,
				format: Array<CompositeFormat>,
			): void {
				for (let e of format) {
					encode(data, image, e);
				}
				return;
			}

			export function decode_n(
				data: Core.IByteStreamView,
				image: Core.Image.VBitmapView,
				format: Array<CompositeFormat>,
			): void {
				for (let e of format) {
					decode(data, image, e);
				}
				return;
			}

			// ------------------------------------------------

			export function encode_fs(
				data_file: string,
				image_file: string,
				format: Array<CompositeFormat>,
			): void {
				let image = Image.File.png_read_file_of(image_file);
				let image_size = image.size().value;
				let data_size = compute_data_size_n(image_size, format);
				let data = Core.ByteArray.allocate(Core.Size.value(data_size));
				let stream = Core.ByteStreamView.look(data.view());
				encode_n(stream, image.view(), format);
				FileSystem.write_file(data_file, stream.stream_view());
				return;
			}

			export function decode_fs(
				data_file: string,
				image_file: string,
				size: Image.ImageSize,
				format: Array<CompositeFormat>,
			): void {
				let data = FileSystem.read_file(data_file);
				let stream = Core.ByteStreamView.look(data.view());
				let image = Core.Image.Bitmap.allocate(Core.Image.ImageSize.value(size));
				let image_view = image.view();
				decode_n(stream, image_view, format);
				Image.File.png_write_file(image_file, image_view);
				return;
			}

			// ------------------------------------------------

		}

		export namespace Wwise {

			export namespace EncodedMedia {

				export function decode_fs(
					ripe_file: string,
					raw_file: string,
					ffmpeg_file: string,
					ww2ogg_file: string,
					ww2ogg_pcb_file: string,
					temporary_directory: string,
				): void {
					let ripe_data = FileSystem.read_file(ripe_file);
					let raw_data = Core.Tool.Wwise.EncodedMedia.Decode.process(ripe_data.view(), Core.Path.value(ffmpeg_file), Core.Path.value(ww2ogg_file), Core.Path.value(ww2ogg_pcb_file), Core.Path.value(temporary_directory));
					FileSystem.write_file(raw_file, raw_data.view());
					return;
				}

			}

			export namespace SoundBank {

				export function pack_fs(
					data_file: string,
					manifest_file: string,
					embedded_audio_directory: string,
					version: Core.Tool.Wwise.SoundBank.JS_Version,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.Wwise.SoundBank.Version.json(Core.JSON.Value.value(version));
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.look(data_buffer_view);
					let manifest = Core.Tool.Wwise.SoundBank.Manifest.SoundBankVariant.from_json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.Wwise.SoundBank.Pack.process(stream, manifest, Core.Path.value(embedded_audio_directory), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					embedded_audio_directory: null | string,
					version: Core.Tool.Wwise.SoundBank.JS_Version,
				): void {
					let version_c = Core.Tool.Wwise.SoundBank.Version.json(Core.JSON.Value.value(version));
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.look(data.view());
					let manifest = Core.Tool.Wwise.SoundBank.Manifest.SoundBankVariant.default();
					Core.Tool.Wwise.SoundBank.Unpack.process(stream, manifest, Core.PathOptional.value(embedded_audio_directory), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.to_json(version_c));
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
					version: Core.Tool.Marmalade.DZip.JS_Version,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.Marmalade.DZip.Version.json(Core.JSON.Value.value(version));
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.look(data_buffer_view);
					let manifest = Core.Tool.Marmalade.DZip.Manifest.PackageVariant.from_json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.Marmalade.DZip.Pack.process(stream, manifest, Core.Path.value(resource_directory), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					resource_directory: null | string,
					version: Core.Tool.Marmalade.DZip.JS_Version,
				): void {
					let version_c = Core.Tool.Marmalade.DZip.Version.json(Core.JSON.Value.value(version));
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.look(data.view());
					let manifest = Core.Tool.Marmalade.DZip.Manifest.PackageVariant.default();
					Core.Tool.Marmalade.DZip.Unpack.process(stream, manifest, Core.PathOptional.value(resource_directory), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.to_json(version_c));
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
					level: Data.Compress.Deflate.CompressionLevel,
					window_bits: Data.Compress.Deflate.WindowBits,
					memory_level: Data.Compress.Deflate.MemoryLevel,
					strategy: Data.Compress.Deflate.Strategy,
					variant_64: boolean,
				): void {
					let raw_data = FileSystem.read_file(raw_file);
					let ripe_size_bound = Core.Tool.PopCap.ZLib.Compress.compute_size_bound(raw_data.size(), Core.Size.value(window_bits), Core.Size.value(memory_level), Core.Boolean.value(variant_64));
					let ripe_data = Core.ByteArray.allocate(ripe_size_bound);
					let raw_stream = Core.ByteStreamView.look(raw_data.view());
					let ripe_stream = Core.ByteStreamView.look(ripe_data.view());
					Core.Tool.PopCap.ZLib.Compress.process(raw_stream, ripe_stream, Core.Size.value(level), Core.Size.value(window_bits), Core.Size.value(memory_level), Core.Tool.Data.Compress.Deflate.Strategy.value(strategy), Core.Boolean.value(variant_64));
					FileSystem.write_file(ripe_file, ripe_stream.stream_view());
					return;
				}

				export function uncompress_fs(
					ripe_file: string,
					raw_file: string,
					window_bits: Data.Compress.Deflate.WindowBits,
					variant_64: boolean,
				): void {
					let ripe_data = FileSystem.read_file(ripe_file);
					let raw_size = Core.Tool.PopCap.ZLib.Uncompress.compute_size(ripe_data.view(), Core.Boolean.value(variant_64));
					let raw_data = Core.ByteArray.allocate(raw_size);
					let ripe_stream = Core.ByteStreamView.look(ripe_data.view());
					let raw_stream = Core.ByteStreamView.look(raw_data.view());
					Core.Tool.PopCap.ZLib.Uncompress.process(ripe_stream, raw_stream, Core.Size.value(window_bits), Core.Boolean.value(variant_64));
					FileSystem.write_file(raw_file, raw_stream.stream_view());
					return;
				}

			}

			export namespace Reanim {

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					version: Core.Tool.PopCap.Reanim.JS_Version,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.Reanim.Version.json(Core.JSON.Value.value(version));
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.look(data_buffer_view);
					let manifest = Core.Tool.PopCap.Reanim.Manifest.AnimationVariant.from_json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.Reanim.Encode.process(stream, manifest, version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: string,
					version: Core.Tool.PopCap.Reanim.JS_Version,
				): void {
					let version_c = Core.Tool.PopCap.Reanim.Version.json(Core.JSON.Value.value(version));
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.look(data.view());
					let manifest = Core.Tool.PopCap.Reanim.Manifest.AnimationVariant.default();
					Core.Tool.PopCap.Reanim.Decode.process(stream, manifest, version_c);
					JSON.write_fs(manifest_file, manifest.to_json(version_c));
					return;
				}

			}

			export namespace RTON {

				export function encode_fs(
					json_file: string,
					rton_file: string,
					enable_string_index: boolean,
					enable_rtid: boolean,
					rton_data_buffer: Core.ByteListView | bigint,
				): void {
					let rton_data_buffer_if = typeof rton_data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(rton_data_buffer)) : null;
					let rton_data_buffer_view = rton_data_buffer instanceof Core.ByteListView ? rton_data_buffer : rton_data_buffer_if!.view();
					let json = JSON.read_fs<Core.Tool.PopCap.RTON.JS_ValidValue>(json_file);
					let rton_stream = Core.ByteStreamView.look(rton_data_buffer_view);
					Core.Tool.PopCap.RTON.Encode.process(rton_stream, json, Core.Boolean.value(enable_string_index), Core.Boolean.value(enable_rtid), Core.Tool.PopCap.RTON.Version.json(Core.JSON.Value.value({ number: 1n })));
					FileSystem.write_file(rton_file, rton_stream.stream_view());
					return;
				}

				export function decode_fs(
					rton_file: string,
					json_file: string,
				): void {
					let rton_data = FileSystem.read_file(rton_file);
					let rton_stream = Core.ByteStreamView.look(rton_data.view());
					let json = Core.JSON.Value.default<Core.Tool.PopCap.RTON.JS_ValidValue>();
					Core.Tool.PopCap.RTON.Decode.process(rton_stream, json, Core.Tool.PopCap.RTON.Version.json(Core.JSON.Value.value({ number: 1n })));
					JSON.write_fs(json_file, json);
					return;
				}

				export function encrypt_fs(
					plain_file: string,
					cipher_file: string,
					key: string,
				): void {
					let plain_data = FileSystem.read_file(plain_file);
					let cipher_size = Core.Tool.PopCap.RTON.Encrypt.compute_size(plain_data.size());
					let cipher_data = Core.ByteArray.allocate(cipher_size);
					let plain_stream = Core.ByteStreamView.look(plain_data.view());
					let cipher_stream = Core.ByteStreamView.look(cipher_data.view());
					Core.Tool.PopCap.RTON.Encrypt.process(plain_stream, cipher_stream, Core.String.value(key));
					FileSystem.write_file(cipher_file, cipher_stream.stream_view());
					return;
				}

				export function decrypt_fs(
					cipher_file: string,
					plain_file: string,
					key: string,
				): void {
					let cipher_data = FileSystem.read_file(cipher_file);
					let plain_size = Core.Tool.PopCap.RTON.Decrypt.compute_size(cipher_data.size());
					let plain_data = Core.ByteArray.allocate(plain_size);
					let cipher_stream = Core.ByteStreamView.look(cipher_data.view());
					let plain_stream = Core.ByteStreamView.look(plain_data.view());
					Core.Tool.PopCap.RTON.Decrypt.process(cipher_stream, plain_stream, Core.String.value(key));
					FileSystem.write_file(plain_file, plain_stream.stream_view());
					return;
				}

				export function encode_then_encrypt_fs(
					json_file: string,
					rton_file: string,
					enable_string_index: boolean,
					enable_rtid: boolean,
					key: string,
					rton_data_buffer: Core.ByteListView | bigint,
				): void {
					let rton_data_buffer_if = typeof rton_data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(rton_data_buffer)) : null;
					let rton_data_buffer_view = rton_data_buffer instanceof Core.ByteListView ? rton_data_buffer : rton_data_buffer_if!.view();
					let json = JSON.read_fs<Core.Tool.PopCap.RTON.JS_ValidValue>(json_file);
					let rton_stream = Core.ByteStreamView.look(rton_data_buffer_view);
					Core.Tool.PopCap.RTON.Encode.process(rton_stream, json, Core.Boolean.value(enable_string_index), Core.Boolean.value(enable_rtid), Core.Tool.PopCap.RTON.Version.json(Core.JSON.Value.value({ number: 1n })));
					let plain_stream = Core.ByteStreamView.look(rton_stream.stream_view());
					let cipher_size = Core.Tool.PopCap.RTON.Encrypt.compute_size(plain_stream.size());
					let cipher_data = Core.ByteArray.allocate(cipher_size);
					let cipher_stream = Core.ByteStreamView.look(cipher_data.view());
					Core.Tool.PopCap.RTON.Encrypt.process(plain_stream, cipher_stream, Core.String.value(key));
					FileSystem.write_file(rton_file, cipher_stream.stream_view());
					return;
				}

				export function decrypt_then_decode_fs(
					rton_file: string,
					json_file: string,
					key: string,
				): void {
					let cipher_data = FileSystem.read_file(rton_file);
					let plain_size = Core.Tool.PopCap.RTON.Decrypt.compute_size(cipher_data.size());
					let plain_data = Core.ByteArray.allocate(plain_size);
					let cipher_stream = Core.ByteStreamView.look(cipher_data.view());
					let plain_stream = Core.ByteStreamView.look(plain_data.view());
					Core.Tool.PopCap.RTON.Decrypt.process(cipher_stream, plain_stream, Core.String.value(key));
					let rton_stream = Core.ByteStreamView.look(plain_stream.stream_view());
					let json = Core.JSON.Value.default<Core.Tool.PopCap.RTON.JS_ValidValue>();
					Core.Tool.PopCap.RTON.Decode.process(rton_stream, json, Core.Tool.PopCap.RTON.Version.json(Core.JSON.Value.value({ number: 1n })));
					JSON.write_fs(json_file, json);
					return;
				}

			}

			export namespace PAM {

				export function encode_fs(
					data_file: string,
					manifest_file: string,
					version: Core.Tool.PopCap.PAM.JS_Version,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.PAM.Version.json(Core.JSON.Value.value(version));
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.look(data_buffer_view);
					let manifest = Core.Tool.PopCap.PAM.Manifest.AnimationVariant.from_json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.PAM.Encode.process(stream, manifest, version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: string,
					manifest_file: string,
					version: Core.Tool.PopCap.PAM.JS_Version,
				): void {
					let version_c = Core.Tool.PopCap.PAM.Version.json(Core.JSON.Value.value(version));
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.look(data.view());
					let manifest = Core.Tool.PopCap.PAM.Manifest.AnimationVariant.default();
					Core.Tool.PopCap.PAM.Decode.process(stream, manifest, version_c);
					JSON.write_fs(manifest_file, manifest.to_json(version_c));
					return;
				}

			}

			export namespace PAK {

				export function pack_fs(
					data_file: string,
					manifest_file: string,
					resource_directory: string,
					version: Core.Tool.PopCap.PAK.JS_Version,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.PAK.Version.json(Core.JSON.Value.value(version));
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.look(data_buffer_view);
					let manifest = Core.Tool.PopCap.PAK.Manifest.PackageVariant.from_json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.PAK.Pack.process(stream, manifest, Core.Path.value(resource_directory), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					resource_directory: null | string,
					version: Core.Tool.PopCap.PAK.JS_Version,
				): void {
					let version_c = Core.Tool.PopCap.PAK.Version.json(Core.JSON.Value.value(version));
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.look(data.view());
					let manifest = Core.Tool.PopCap.PAK.Manifest.PackageVariant.default();
					Core.Tool.PopCap.PAK.Unpack.process(stream, manifest, Core.PathOptional.value(resource_directory), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.to_json(version_c));
					}
					return;
				}

			}

			export namespace RSGP {

				export function pack_fs(
					data_file: string,
					manifest_file: string,
					resource_directory: string,
					version: Core.Tool.PopCap.RSGP.JS_Version,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.RSGP.Version.json(Core.JSON.Value.value(version));
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.look(data_buffer_view);
					let manifest = Core.Tool.PopCap.RSGP.Manifest.PackageVariant.from_json(JSON.read_fs(manifest_file), version_c);
					Core.Tool.PopCap.RSGP.Pack.process(stream, manifest, Core.Path.value(resource_directory), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					resource_directory: null | string,
					version: Core.Tool.PopCap.RSGP.JS_Version,
				): void {
					let version_c = Core.Tool.PopCap.RSGP.Version.json(Core.JSON.Value.value(version));
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.look(data.view());
					let manifest = Core.Tool.PopCap.RSGP.Manifest.PackageVariant.default();
					Core.Tool.PopCap.RSGP.Unpack.process(stream, manifest, Core.PathOptional.value(resource_directory), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.to_json(version_c));
					}
					return;
				}

			}

			export namespace RSB {

				export function pack_fs(
					data_file: string,
					manifest_file: string,
					description_file: string,
					resource_directory: string,
					packet_file: null | string,
					new_packet_file: null | string,
					version: Core.Tool.PopCap.RSB.JS_Version,
					data_buffer: Core.ByteListView | bigint,
				): void {
					let version_c = Core.Tool.PopCap.RSB.Version.json(Core.JSON.Value.value(version));
					let data_buffer_if = typeof data_buffer === 'bigint' ? Core.ByteArray.allocate(Core.Size.value(data_buffer)) : null;
					let data_buffer_view = data_buffer instanceof Core.ByteListView ? data_buffer : data_buffer_if!.view();
					let stream = Core.ByteStreamView.look(data_buffer_view);
					let manifest = Core.Tool.PopCap.RSB.Manifest.PackageVariant.from_json(JSON.read_fs(manifest_file), version_c);
					let description = Core.Tool.PopCap.RSB.Description.PackageOptionalVariant.from_json(JSON.read_fs(description_file), version_c);
					Core.Tool.PopCap.RSB.Pack.process(stream, manifest, description, Core.Path.value(resource_directory), Core.PathOptional.value(packet_file), Core.PathOptional.value(new_packet_file), version_c);
					FileSystem.write_file(data_file, stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: string,
					manifest_file: null | string,
					description_file: null | string,
					resource_directory: null | string,
					packet_file: null | string,
					version: Core.Tool.PopCap.RSB.JS_Version,
				): void {
					let version_c = Core.Tool.PopCap.RSB.Version.json(Core.JSON.Value.value(version));
					let data = FileSystem.read_file(data_file);
					let stream = Core.ByteStreamView.look(data.view());
					let manifest = Core.Tool.PopCap.RSB.Manifest.PackageVariant.default();
					let description = Core.Tool.PopCap.RSB.Description.PackageOptionalVariant.default();
					Core.Tool.PopCap.RSB.Unpack.process(stream, manifest, description, Core.PathOptional.value(resource_directory), Core.PathOptional.value(packet_file), version_c);
					if (manifest_file !== null) {
						JSON.write_fs(manifest_file, manifest.to_json(version_c));
					}
					if (description_file !== null) {
						JSON.write_fs(description_file, description.to_json(version_c));
					}
					return;
				}

			}

		}

		export namespace Misc {

			export namespace PvZ2CHSRSBTextureAlphaIndex {

				// ------------------------------------------------

				export type BitCount = 1 | 2 | 3 | 4;

				export function compute_bit_count(
					value_count: number,
				): BitCount {
					if (value_count <= 0b1 || value_count > 0b10000) {
						throw new MyError(`invalue value count`);
					}
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

				export function compute_data_size_with_index_list(
					size: Image.ImageSize,
					index_count: number,
				): bigint {
					let bit_count = compute_bit_count(index_count);
					return 1n + BigInt(index_count === 2 ? 0 : index_count) + size[0] * size[1] * BigInt(bit_count) / 8n;
				}

				export function evaluate_index_list(
					image: Core.Image.CBitmapView,
				): Core.Tool.Misc.PvZ2CHSRSBTextureAlphaIndex.JS_IndexList {
					let image_size = image.size().value;
					let image_data = Core.ByteArray.allocate(Core.Size.value(image_size[0] * image_size[1] * 8n / 8n));
					let image_stream = Core.ByteStreamView.look(image_data.view());
					Texture.encode(image_stream, image, 'a_8');
					let alpha_count: Record<number, number> = {};
					for (let e of new Uint8Array(image_stream.stream_view().value)) {
						let alpha_4 = (e >> 4) & ~(~0 << 4);
						alpha_count[alpha_4] = (alpha_count[alpha_4] || 0) + 1;
					}
					let index_list = Object.keys(alpha_count).map(BigInt);
					if (index_list.length <= 2) {
						if (!index_list.includes(0b0000n)) {
							index_list.push(0b0000n);
						}
						if (!index_list.includes(0b1111n)) {
							index_list.push(0b1111n);
						}
						if (index_list.length === 2) {
							index_list = [0b0000n, 0b1111n];
						}
					}
					return index_list;
				}

				// ------------------------------------------------

				export function encode_with_map(
					data: Core.OByteStreamView,
					image: Core.Image.CBitmapView,
					index: Core.Tool.Misc.PvZ2CHSRSBTextureAlphaIndex.JS_IndexList,
				): void {
					let bit_count = compute_bit_count(index.length);
					if (bit_count === 1) {
						data.write(Core.Byte.value(0n));
					} else {
						data.write(Core.Byte.value(BigInt(index.length)));
						for (let e of index) {
							data.write(Core.Byte.value(e));
						}
					}
					Core.Tool.Misc.PvZ2CHSRSBTextureAlphaIndex.Encode.process(data, image, index);
					return;
				}

				export function decode_with_map(
					data: Core.IByteStreamView,
					image: Core.Image.VBitmapView,
				): void {
					let index_count = data.read().value;
					let index: Core.Tool.Misc.PvZ2CHSRSBTextureAlphaIndex.JS_IndexList;
					if (index_count === 0n) {
						index = [0b0000n, 0b1111n];
					} else {
						index = [];
						for (let i = 0n; i < index_count; ++i) {
							index.push(data.read().value);
						}
					}
					Core.Tool.Misc.PvZ2CHSRSBTextureAlphaIndex.Decode.process(data, image, index);
					return;
				}

				// ------------------------------------------------

			}

		}

	}

	export namespace Misc {

		export function evaluate(
			script: string,
			name: string = '<!evaluate>',
		): any {
			let script_s = Core.String.value(script);
			return Core.Misc.g_context.evaluate(Core.Misc.cast_String_to_CharacterListView(script_s), Core.String.value(name));
		}

		export function evaluate_fs(
			script_file: string,
			name: string = script_file,
		): any {
			let script = FileSystem.read_file(script_file);
			return Core.Misc.g_context.evaluate(Core.Misc.cast_ByteListView_to_CharacterListView(script.view()), Core.String.value(name));
		}

		export function shell_callback(
			argument: Array<string>,
		): Array<string> {
			return Core.Misc.g_context.shell_callback(Core.StringList.value(argument)).value;
		}

	}

}