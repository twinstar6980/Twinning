namespace Twinning.Script.KernelX {

	// #region common

	export const system = Kernel.Miscellaneous.g_system.value;

	export const architecture = Kernel.Miscellaneous.g_architecture.value;

	// ----------------

	export const is_windows = system === 'windows';

	export const is_linux = system === 'linux';

	export const is_macintosh = system === 'macintosh';

	export const is_android = system === 'android';

	export const is_iphone = system === 'iphone';

	// ----------------

	export let g_common_buffer: Kernel.ByteArray = Kernel.ByteArray.default();

	export let g_common_buffer_for_string: Kernel.ByteArray = Kernel.ByteArray.allocate(Kernel.Size.value(0x2000n));

	// #endregion

	// #region wrapper

	export namespace Image {

		export type ImageSize = typeof Kernel.Image.ImageSize.Value;

		export type ImagePosition = typeof Kernel.Image.ImagePosition.Value;

		// ----------------

		export type Color = typeof Kernel.Image.Color.Value;

		export type ColorList = typeof Kernel.Image.ColorList.Value;

		export type Pixel = typeof Kernel.Image.Pixel.Value;

	}

	// ----------------

	export namespace Storage {

		export function exist(
			target: StoragePath,
		): boolean {
			return Kernel.Storage.exist(Kernel.Path.value(target.emit())).value;
		}

		export function copy(
			target: StoragePath,
			placement: StoragePath,
			follow_link: boolean,
		): void {
			return Kernel.Storage.copy(Kernel.Path.value(target.emit()), Kernel.Path.value(placement.emit()), Kernel.Boolean.value(follow_link));
		}

		export function rename(
			target: StoragePath,
			placement: StoragePath,
		): void {
			return Kernel.Storage.rename(Kernel.Path.value(target.emit()), Kernel.Path.value(placement.emit()));
		}

		export function remove(
			target: StoragePath,
		): void {
			return Kernel.Storage.remove(Kernel.Path.value(target.emit()));
		}

		// ----------------

		export function exist_link(
			target: StoragePath,
		): boolean {
			return Kernel.Storage.exist_link(Kernel.Path.value(target.emit())).value;
		}

		export function create_link(
			target: StoragePath,
			referent: StoragePath,
			is_directory: boolean,
		): void {
			return Kernel.Storage.create_link(Kernel.Path.value(target.emit()), Kernel.Path.value(referent.emit()), Kernel.Boolean.value(is_directory));
		}

		export function resolve_link(
			target: StoragePath,
		): string {
			return Kernel.Storage.resolve_link(Kernel.Path.value(target.emit())).value;
		}

		// ----------------

		export function exist_file(
			target: StoragePath,
		): boolean {
			return Kernel.Storage.exist_file(Kernel.Path.value(target.emit())).value;
		}

		export function create_file(
			target: StoragePath,
		): void {
			return Kernel.Storage.create_file(Kernel.Path.value(target.emit()));
		}

		export function size_file(
			target: StoragePath,
		): bigint {
			return Kernel.Storage.size_file(Kernel.Path.value(target.emit())).value;
		}

		export function read_file(
			target: StoragePath,
		): Kernel.ByteArray {
			return Kernel.Storage.read_file(Kernel.Path.value(target.emit()));
		}

		export function write_file(
			target: StoragePath,
			data: Kernel.ByteListView,
		): void {
			return Kernel.Storage.write_file(Kernel.Path.value(target.emit()), data);
		}

		// ----------------

		export function exist_directory(
			target: StoragePath,
		): boolean {
			return Kernel.Storage.exist_directory(Kernel.Path.value(target.emit())).value;
		}

		export function create_directory(
			target: StoragePath,
		): void {
			return Kernel.Storage.create_directory(Kernel.Path.value(target.emit()));
		}

		export function list_directory(
			target: StoragePath,
			depth: null | bigint,
			follow_link: boolean,
			allow_link: boolean,
			allow_file: boolean,
			allow_directory: boolean,
		): Array<StoragePath> {
			return Kernel.Storage.list_directory(Kernel.Path.value(target.emit()), Kernel.SizeOptional.value(depth), Kernel.Boolean.value(follow_link), Kernel.Boolean.value(allow_link), Kernel.Boolean.value(allow_file), Kernel.Boolean.value(allow_directory)).value.map((it) => new StoragePath(it));
		}

	}

	export namespace Process {

		export function get_workspace(
		): StoragePath {
			return new StoragePath(Kernel.Process.get_workspace().value);
		}

		// ----------------

		export function get_environment(
		): Array<string> {
			return Kernel.Process.get_environment().value;
		}

		export function find_environment(
			name: string,
		): null | string {
			return Kernel.Process.find_environment(Kernel.String.value(name)).value;
		}

		// ----------------

		export function run_process(
			program: StoragePath,
			argument: Array<string>,
			workspace: StoragePath,
			environment: Array<string>,
			input: null | StoragePath,
			output: null | StoragePath,
			error: null | StoragePath,
		): bigint {
			return Kernel.Process.run_process(Kernel.Path.value(program.emit()), Kernel.StringList.value(argument), Kernel.Path.value(workspace.emit()), Kernel.StringList.value(environment), Kernel.PathOptional.value(input === null ? null : input.emit()), Kernel.PathOptional.value(output === null ? null : output.emit()), Kernel.PathOptional.value(error === null ? null : error.emit())).value;
		}

	}

	// ----------------

	export namespace Tool {

		export namespace Data {

			export namespace Hashing {

				export namespace Md5 {

					export function hash_fs(
						data_file: StoragePath,
					): bigint {
						let data = StorageHelper.read_file(data_file);
						let value = Kernel.ByteArray.default();
						Kernel.Tool.Data.Hashing.Md5.Hash.process(data.view(), value);
						return ConvertHelper.parse_integer_from_byte_array([...new Uint8Array(value.value)].map(BigInt), null, 'little');
					}

				}

				export namespace Fnv {

					const ModeX = ['m_0', 'm_1', 'm_1a'] as const;

					export type Mode = typeof ModeX[number];

					export const ModeE = ModeX as unknown as Mode[];

					const BitCountX = ['b_32', 'b_64'] as const;

					export type BitCount = typeof BitCountX[number];

					export const BitCountE = BitCountX as unknown as BitCount[];

					// ----------------

					export function hash_s(
						data_string: string,
						mode: Mode,
						bit_count: BitCount,
					): bigint {
						let data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(data_string));
						let value = Kernel.ByteArray.default();
						Kernel.Tool.Data.Hashing.Fnv.Hash.process(data.view(), value, Kernel.Tool.Data.Hashing.Fnv.Mode.value(mode), Kernel.Tool.Data.Hashing.Fnv.BitCount.value(bit_count));
						return ConvertHelper.parse_integer_from_byte_array([...new Uint8Array(value.value)].map(BigInt), null, 'current');
					}

					export function hash_fs(
						data_file: StoragePath,
						mode: Mode,
						bit_count: BitCount,
					): bigint {
						let data = StorageHelper.read_file(data_file);
						let value = Kernel.ByteArray.default();
						Kernel.Tool.Data.Hashing.Fnv.Hash.process(data.view(), value, Kernel.Tool.Data.Hashing.Fnv.Mode.value(mode), Kernel.Tool.Data.Hashing.Fnv.BitCount.value(bit_count));
						return ConvertHelper.parse_integer_from_byte_array([...new Uint8Array(value.value)].map(BigInt), null, 'current');
					}

				}

			}

			export namespace Encoding {

				export namespace Base64 {

					export function encode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
					): void {
						let raw = StorageHelper.read_file(raw_file);
						let ripe_size = Kernel.Size.default();
						Kernel.Tool.Data.Encoding.Base64.Encode.estimate(raw.size(), ripe_size);
						let ripe = Kernel.ByteArray.allocate(ripe_size);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						Kernel.Tool.Data.Encoding.Base64.Encode.process(raw_stream, ripe_stream);
						StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function decode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
					): void {
						let ripe = StorageHelper.read_file(ripe_file);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let raw_size = Kernel.Size.default();
						Kernel.Tool.Data.Encoding.Base64.Decode.estimate(raw_size, ripe_stream.view());
						let raw = Kernel.ByteArray.allocate(raw_size);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						Kernel.Tool.Data.Encoding.Base64.Decode.process(raw_stream, ripe_stream);
						StorageHelper.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

			}

			export namespace Encryption {

				export namespace Exor {

					export function encrypt_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						key: Array<bigint>,
					): void {
						let raw = StorageHelper.read_file(raw_file);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe = Kernel.ByteArray.allocate(raw.size());
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let key_c = Kernel.ByteArray.allocate(Kernel.Size.value(BigInt(key.length)));
						new Uint8Array(key_c.view().value).set(key.map(Number));
						Kernel.Tool.Data.Encryption.Exor.Encrypt.process(raw_stream, ripe_stream, key_c.view());
						StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

				}

				export namespace Rijndael {

					const ModeX = ['ecb', 'cbc', 'cfb'] as const;

					export type Mode = typeof ModeX[number];

					export const ModeE = ModeX as unknown as Mode[];

					const BlockSizeX = [16n, 24n, 32n] as const;

					export type BlockSize = typeof BlockSizeX[number];

					export const BlockSizeE = BlockSizeX as unknown as BlockSize[];

					// ----------------

					export function encrypt_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						mode: Mode,
						block_size: BlockSize,
						key: Kernel.ByteListView,
						initialization_vector: Kernel.ByteListView,
					): void {
						let raw = StorageHelper.read_file(raw_file);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe = Kernel.ByteArray.allocate(raw.size());
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						Kernel.Tool.Data.Encryption.Rijndael.Encrypt.process(raw_stream, ripe_stream, Kernel.Tool.Data.Encryption.Rijndael.Mode.value(mode), Kernel.Integer.value(block_size), key, initialization_vector);
						StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function decrypt_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						mode: Mode,
						block_size: BlockSize,
						key: Kernel.ByteListView,
						initialization_vector: Kernel.ByteListView,
					): void {
						let ripe = StorageHelper.read_file(ripe_file);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let raw = Kernel.ByteArray.allocate(ripe.size());
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						Kernel.Tool.Data.Encryption.Rijndael.Decrypt.process(raw_stream, ripe_stream, Kernel.Tool.Data.Encryption.Rijndael.Mode.value(mode), Kernel.Integer.value(block_size), key, initialization_vector);
						StorageHelper.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

			}

			export namespace Compression {

				export namespace Deflate {

					const LevelX = [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type Level = typeof LevelX[number];

					export const LevelE = LevelX as unknown as Level[];

					const WindowExponentX = [8n, 9n, 10n, 11n, 12n, 13n, 14n, 15n] as const;

					export type WindowExponent = typeof WindowExponentX[number];

					export const WindowExponentE = WindowExponentX as unknown as WindowExponent[];

					const MemoryLevelX = [1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type MemoryLevel = typeof MemoryLevelX[number];

					export const MemoryLevelE = MemoryLevelX as unknown as MemoryLevel[];

					const StrategyModeX = ['default_mode', 'filtered', 'huffman_only', 'rle', 'fixed'] as const;

					export type StrategyMode = typeof StrategyModeX[number];

					export const StrategyModeE = StrategyModeX as unknown as StrategyMode[];

					const WrapperTypeX = ['none', 'zlib', 'gzip'] as const;

					export type WrapperType = typeof WrapperTypeX[number];

					export const WrapperTypeE = WrapperTypeX as unknown as WrapperType[];

					// ----------------

					export function compress_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						level: Level,
						window_exponent: WindowExponent,
						memory_level: MemoryLevel,
						strategy_mode: StrategyMode,
						wrapper_type: WrapperType,
					): void {
						let raw = StorageHelper.read_file(raw_file);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe_size_bound = Kernel.Size.default();
						Kernel.Tool.Data.Compression.Deflate.Compress.estimate(raw.size(), ripe_size_bound, Kernel.Integer.value(window_exponent), Kernel.Integer.value(memory_level), Kernel.Tool.Data.Compression.Deflate.WrapperType.value(wrapper_type));
						let ripe = Kernel.ByteArray.allocate(ripe_size_bound);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						Kernel.Tool.Data.Compression.Deflate.Compress.process(raw_stream, ripe_stream, Kernel.Integer.value(level), Kernel.Integer.value(window_exponent), Kernel.Integer.value(memory_level), Kernel.Tool.Data.Compression.Deflate.StrategyMode.value(strategy_mode), Kernel.Tool.Data.Compression.Deflate.WrapperType.value(wrapper_type));
						StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						window_exponent: WindowExponent,
						wrapper_type: WrapperType,
						raw_buffer: Kernel.ByteListView,
					): void {
						let ripe = StorageHelper.read_file(ripe_file);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let raw_stream = Kernel.ByteStreamView.watch(raw_buffer);
						Kernel.Tool.Data.Compression.Deflate.Uncompress.process(raw_stream, ripe_stream, Kernel.Integer.value(window_exponent), Kernel.Tool.Data.Compression.Deflate.WrapperType.value(wrapper_type));
						StorageHelper.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

				export namespace Bzip2 {

					const BlockSizeX = [1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type BlockSize = typeof BlockSizeX[number];

					export const BlockSizeE = BlockSizeX as unknown as BlockSize[];

					// ----------------

					export function compress_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						block_size: BlockSize,
					): void {
						let raw = StorageHelper.read_file(raw_file);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe_size_bound = Kernel.Size.value(raw.size().value + 128n); // TODO
						let ripe = Kernel.ByteArray.allocate(ripe_size_bound);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						Kernel.Tool.Data.Compression.Bzip2.Compress.process(raw_stream, ripe_stream, Kernel.Integer.value(block_size), Kernel.Integer.value(0n));
						StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						raw_buffer: Kernel.ByteListView,
					): void {
						let ripe = StorageHelper.read_file(ripe_file);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let raw_stream = Kernel.ByteStreamView.watch(raw_buffer);
						Kernel.Tool.Data.Compression.Bzip2.Uncompress.process(raw_stream, ripe_stream, Kernel.Boolean.value(false));
						StorageHelper.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

				export namespace Lzma {

					const LevelX = [0n, 1n, 2n, 3n, 4n, 5n, 6n, 7n, 8n, 9n] as const;

					export type Level = typeof LevelX[number];

					export const LevelE = LevelX as unknown as Level[];

					// ----------------

					export function compress_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						level: Level,
					): void {
						let raw = StorageHelper.read_file(raw_file);
						let ripe_size_bound = Kernel.Size.value(5n + BigInt(Math.ceil(Number(raw.size().value) * 1.1)) + 1024n * 16n);
						let ripe = Kernel.ByteArray.allocate(ripe_size_bound);
						let raw_stream = Kernel.ByteStreamView.watch(raw.view());
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						Kernel.Tool.Data.Compression.Lzma.Compress.process(raw_stream, ripe_stream, Kernel.Integer.value(level));
						StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
						return;
					}

					export function uncompress_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						raw_buffer: Kernel.ByteListView,
					): void {
						let ripe = StorageHelper.read_file(ripe_file);
						let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
						let raw_stream = Kernel.ByteStreamView.watch(raw_buffer);
						Kernel.Tool.Data.Compression.Lzma.Uncompress.process(raw_stream, ripe_stream);
						StorageHelper.write_file(raw_file, raw_stream.stream_view());
						return;
					}

				}

			}

			export namespace Differentiation {

				export namespace Vcdiff {

					export function encode_fs(
						before_file: StoragePath,
						after_file: StoragePath,
						patch_file: StoragePath,
						interleaved: boolean,
						patch_size_bound: bigint,
					): void {
						let before = StorageHelper.read_file(before_file);
						let before_stream = Kernel.ByteStreamView.watch(before.view());
						let after = StorageHelper.read_file(after_file);
						let after_stream = Kernel.ByteStreamView.watch(after.view());
						let patch = Kernel.ByteArray.allocate(Kernel.Size.value(patch_size_bound));
						let patch_stream = Kernel.ByteStreamView.watch(patch.view());
						Kernel.Tool.Data.Differentiation.Vcdiff.Encode.process(before_stream, after_stream, patch_stream, Kernel.Boolean.value(interleaved));
						StorageHelper.write_file(patch_file, patch_stream.stream_view());
						return;
					}

					export function decode_fs(
						before_file: StoragePath,
						after_file: StoragePath,
						patch_file: StoragePath,
						maximum_window_size: bigint,
						after_size_bound: bigint,
					): void {
						let before = StorageHelper.read_file(before_file);
						let before_stream = Kernel.ByteStreamView.watch(before.view());
						let after = Kernel.ByteArray.allocate(Kernel.Size.value(after_size_bound));
						let after_stream = Kernel.ByteStreamView.watch(after.view());
						let patch = StorageHelper.read_file(patch_file);
						let patch_stream = Kernel.ByteStreamView.watch(patch.view());
						Kernel.Tool.Data.Differentiation.Vcdiff.Decode.process(before_stream, after_stream, patch_stream, Kernel.Integer.value(maximum_window_size));
						StorageHelper.write_file(after_file, after_stream.stream_view());
						return;
					}

				}

			}

			export namespace Serialization {
				
				export namespace Json {

					export let g_format: {
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

					// ----------------

					/** result is a view of buffer */
					export function encode<TConstraint extends Kernel.Notation.Json.JS_Value>(
						value: Kernel.Notation.Json.Value<TConstraint>,
						disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
						disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
						disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
						disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
						data_buffer: Kernel.CharacterListView = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
					): ArrayBuffer {
						let data_stream = Kernel.CharacterStreamView.watch(data_buffer);
						Kernel.Tool.Data.Serialization.Json.Encode.process(data_stream, value, Kernel.Boolean.value(disable_array_trailing_comma), Kernel.Boolean.value(disable_array_line_breaking), Kernel.Boolean.value(disable_object_trailing_comma), Kernel.Boolean.value(disable_object_line_breaking));
						return Kernel.Miscellaneous.cast_CharacterListView_to_ByteListView(data_stream.stream_view()).value;
					}

					export function decode<TConstraint extends Kernel.Notation.Json.JS_Value>(
						data: ArrayBuffer,
					): Kernel.Notation.Json.Value<TConstraint> {
						let data_stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(Kernel.ByteListView.value(data)));
						let value = Kernel.Notation.Json.Value.default<TConstraint>();
						let buffer_stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer_for_string.view()));
						Kernel.Tool.Data.Serialization.Json.Decode.process(data_stream, value, buffer_stream);
						return value;
					}

					// ----------------

					export function encode_s<TConstraint extends Kernel.Notation.Json.JS_Value>(
						value: Kernel.Notation.Json.Value<TConstraint>,
						disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
						disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
						disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
						disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
						data_buffer: Kernel.CharacterListView = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
					): string {
						let data = encode(value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
						return Kernel.Miscellaneous.cast_CharacterListView_to_JS_String(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(Kernel.ByteListView.value(data)));
					}

					export function decode_s<TConstraint extends Kernel.Notation.Json.JS_Value>(
						data: string,
					): Kernel.Notation.Json.Value<TConstraint> {
						let data_byte = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(data));
						return decode(data_byte.view().value);
					}

					// ----------------

					export function encode_fs<TConstraint extends Kernel.Notation.Json.JS_Value>(
						data_file: StoragePath,
						value: Kernel.Notation.Json.Value<TConstraint>,
						disable_array_trailing_comma: boolean = g_format.disable_array_trailing_comma,
						disable_array_line_breaking: boolean = g_format.disable_array_line_breaking,
						disable_object_trailing_comma: boolean = g_format.disable_object_trailing_comma,
						disable_object_line_breaking: boolean = g_format.disable_object_line_breaking,
						data_buffer: Kernel.CharacterListView = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(g_common_buffer.view()),
					): void {
						let data = encode(value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
						StorageHelper.write_file(data_file, data);
						return;
					}

					export function decode_fs<TConstraint extends Kernel.Notation.Json.JS_Value>(
						data_file: StoragePath,
					): Kernel.Notation.Json.Value<TConstraint> {
						let data = StorageHelper.read_file(data_file);
						return decode(data.view().value);
					}

				}

				export namespace Xml {

					export function encode_fs(
						data_file: StoragePath,
						value: Kernel.Notation.Xml.Node,
					): void {
						let data = Kernel.String.default();
						Kernel.Tool.Data.Serialization.Xml.Encode.process(data, value);
						let data_byte = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(data);
						StorageHelper.write_file(data_file, data_byte.view());
						return;
					}

					export function decode_fs(
						data_file: StoragePath,
					): Kernel.Notation.Xml.Node {
						let data_byte = StorageHelper.read_file(data_file);
						let data = Kernel.Miscellaneous.cast_moveable_ByteArray_to_String(data_byte);
						let value = Kernel.Notation.Xml.Node.default();
						Kernel.Tool.Data.Serialization.Xml.Decode.process(data, value);
						return value;
					}

				}

			}

		}

		export namespace Texture {

			export namespace Transformation {

				export namespace Filling {

					export function encode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
						fill_red: null | bigint,
						fill_green: null | bigint,
						fill_blue: null | bigint,
						fill_alpha: null | bigint,
					): void {
						return Kernel.Tool.Texture.Transformation.Filling.Encode.process(raw, ripe, Kernel.Image.ColorOptional.value(fill_red), Kernel.Image.ColorOptional.value(fill_green), Kernel.Image.ColorOptional.value(fill_blue), Kernel.Image.ColorOptional.value(fill_alpha));
					}

					// ----------------

					export function encode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						fill_red: null | bigint,
						fill_green: null | bigint,
						fill_blue: null | bigint,
						fill_alpha: null | bigint,
					): void {
						let raw = Conversion.Png.decode_fs_of(raw_file);
						let raw_view = raw.view();
						let ripe = Kernel.Image.Image.allocate(raw_view.size());
						let ripe_view = ripe.view();
						encode(raw_view, ripe_view, fill_red, fill_green, fill_blue, fill_alpha);
						Conversion.Png.encode_fs(ripe_file, ripe_view);
						return;
					}

				}

				export namespace Flipping {

					export function encode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
						flip_horizontal: boolean,
						flip_vertical: boolean,
					): void {
						return Kernel.Tool.Texture.Transformation.Flipping.Encode.process(raw, ripe, Kernel.Boolean.value(flip_horizontal), Kernel.Boolean.value(flip_vertical));
					}

					// ----------------

					export function encode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						flip_horizontal: boolean,
						flip_vertical: boolean,
					): void {
						let raw = Conversion.Png.decode_fs_of(raw_file);
						let raw_view = raw.view();
						let ripe = Kernel.Image.Image.allocate(raw_view.size());
						let ripe_view = ripe.view();
						encode(raw_view, ripe_view, flip_horizontal, flip_vertical);
						Conversion.Png.encode_fs(ripe_file, ripe_view);
						return;
					}

				}

				export namespace Rotating {

					export function encode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
						rotate_90: boolean,
						rotate_180: boolean,
					): void {
						return Kernel.Tool.Texture.Transformation.Rotating.Encode.process(raw, ripe, Kernel.Boolean.value(rotate_90), Kernel.Boolean.value(rotate_180));
					}

					// ----------------

					export function encode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						rotate_90: boolean,
						rotate_180: boolean,
					): void {
						let raw = Conversion.Png.decode_fs_of(raw_file);
						let raw_view = raw.view();
						let ripe = Kernel.Image.Image.allocate(!rotate_90 ? raw_view.size() : Kernel.Image.ImageSize.value([raw_view.size().value[1], raw_view.size().value[0]]));
						let ripe_view = ripe.view();
						encode(raw_view, ripe_view, rotate_90, rotate_180);
						Conversion.Png.encode_fs(ripe_file, ripe_view);
						return;
					}

				}

				export namespace Scaling {

					export function encode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
					): void {
						return Kernel.Tool.Texture.Transformation.Scaling.Encode.process(raw, ripe);
					}

					// ----------------

					export function encode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						target_size: Image.ImageSize,
					): void {
						let raw = Conversion.Png.decode_fs_of(raw_file);
						let raw_view = raw.view();
						let ripe = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(target_size));
						let ripe_view = ripe.view();
						encode(raw_view, ripe_view);
						Conversion.Png.encode_fs(ripe_file, ripe_view);
						return;
					}

					export function encode_rate_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						target_rate: number,
					): void {
						let raw = Conversion.Png.decode_fs_of(raw_file);
						let raw_view = raw.view();
						let target_size = Kernel.Image.ImageSize.value([BigInt(Math.max(1, Math.round(Number(raw.size().value[0]) * target_rate))), BigInt(Math.max(1, Math.round(Number(raw.size().value[1]) * target_rate)))]);
						let ripe = Kernel.Image.Image.allocate(target_size);
						let ripe_view = ripe.view();
						encode(raw_view, ripe_view);
						Conversion.Png.encode_fs(ripe_file, ripe_view);
						return;
					}

				}

				export namespace Tiling {

					export function encode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
						tile_size: Image.ImageSize,
					): void {
						return Kernel.Tool.Texture.Transformation.Tiling.Encode.process(raw, ripe, Kernel.Image.ImageSize.value(tile_size));
					}

					export function decode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
						tile_size: Image.ImageSize,
					): void {
						return Kernel.Tool.Texture.Transformation.Tiling.Decode.process(raw, ripe, Kernel.Image.ImageSize.value(tile_size));
					}

					// ----------------

					export function encode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						tile_size: Image.ImageSize,
					): void {
						let raw = Conversion.Png.decode_fs_of(raw_file);
						let raw_view = raw.view();
						let ripe = Kernel.Image.Image.allocate(raw.size());
						let ripe_view = ripe.view();
						encode(raw_view, ripe_view, tile_size);
						Conversion.Png.encode_fs(ripe_file, ripe_view);
						return;
					}

					export function decode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
						tile_size: Image.ImageSize,
					): void {
						let ripe = Conversion.Png.decode_fs_of(ripe_file);
						let ripe_view = ripe.view();
						let raw = Kernel.Image.Image.allocate(ripe.size());
						let raw_view = raw.view();
						decode(raw_view, ripe_view, tile_size);
						Conversion.Png.encode_fs(raw_file, raw_view);
						return;
					}

				}

				export namespace Interleaving {

					export function encode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
					): void {
						return Kernel.Tool.Texture.Transformation.Interleaving.Encode.process(raw, ripe);
					}

					export function decode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
					): void {
						return Kernel.Tool.Texture.Transformation.Interleaving.Decode.process(raw, ripe);
					}

					// ----------------

					export function encode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
					): void {
						let raw = Conversion.Png.decode_fs_of(raw_file);
						let raw_view = raw.view();
						let ripe = Kernel.Image.Image.allocate(raw.size());
						let ripe_view = ripe.view();
						encode(raw_view, ripe_view);
						Conversion.Png.encode_fs(ripe_file, ripe_view);
						return;
					}

					export function decode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
					): void {
						let ripe = Conversion.Png.decode_fs_of(ripe_file);
						let ripe_view = ripe.view();
						let raw = Kernel.Image.Image.allocate(ripe.size());
						let raw_view = raw.view();
						decode(raw_view, ripe_view);
						Conversion.Png.encode_fs(raw_file, raw_view);
						return;
					}

				}

				export namespace Premultiplying {

					export function encode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
					): void {
						return Kernel.Tool.Texture.Transformation.Premultiplying.Encode.process(raw, ripe);
					}

					export function decode(
						raw: Kernel.Image.ConstantImageView,
						ripe: Kernel.Image.VariableImageView,
					): void {
						return Kernel.Tool.Texture.Transformation.Premultiplying.Decode.process(raw, ripe);
					}

					// ----------------

					export function encode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
					): void {
						let raw = Conversion.Png.decode_fs_of(raw_file);
						let raw_view = raw.view();
						let ripe = Kernel.Image.Image.allocate(raw.size());
						let ripe_view = ripe.view();
						encode(raw_view, ripe_view);
						Conversion.Png.encode_fs(ripe_file, ripe_view);
						return;
					}

					export function decode_fs(
						raw_file: StoragePath,
						ripe_file: StoragePath,
					): void {
						let ripe = Conversion.Png.decode_fs_of(ripe_file);
						let ripe_view = ripe.view();
						let raw = Kernel.Image.Image.allocate(ripe.size());
						let raw_view = raw.view();
						decode(raw_view, ripe_view);
						Conversion.Png.encode_fs(raw_file, raw_view);
						return;
					}

				}

			}

			export namespace Encoding {

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
					'rgb_888_r',
					'rgba_8888_o',
					'rgba_8888_r',
					'argb_8888_o',
					'argb_8888_r',
				] as const;

				export type Format = typeof FormatX[number];

				export const FormatE = FormatX as unknown as Format[];

				const CompressionX = [
					'rgba_dxtc1',
					'rgba_dxtc3',
					'rgba_dxtc3_p',
					'rgba_dxtc5',
					'rgba_dxtc5_p',
					'rgb_pvrtc1_4bpp',
					'rgba_pvrtc1_4bpp',
					'rgb_etc1',
					'rgb_etc2',
					'rgba_etc2_eac',
					'r_eac',
					'rg_eac',
					'rgba_astc_4x4',
					'rgba_astc_5x4',
					'rgba_astc_5x5',
					'rgba_astc_6x5',
					'rgba_astc_6x6',
					'rgba_astc_8x5',
					'rgba_astc_8x6',
					'rgba_astc_8x8',
					'rgba_astc_10x5',
					'rgba_astc_10x6',
					'rgba_astc_10x8',
					'rgba_astc_10x10',
					'rgba_astc_12x10',
					'rgba_astc_12x12',
				] as const;

				export type Compression = typeof CompressionX[number];

				export const CompressionE = CompressionX as unknown as Compression[];

				const CompositeFormatX = [
					...FormatX,
					...CompressionX,
				] as const;

				export type CompositeFormat = typeof CompositeFormatX[number];

				export const CompositeFormatE = CompositeFormatX as unknown as CompositeFormat[];

				// ----------------

				export function get_dummy_channel_filler(
					format: CompositeFormat,
				): [null | bigint, null | bigint, null | bigint, null | bigint] {
					let channel: Array<string>;
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
						case 'rgb_888_r':
						case 'rgba_8888_o':
						case 'rgba_8888_r':
						case 'argb_8888_o':
						case 'argb_8888_r': {
							channel = format.split('_')[0].split('');
							break;
						}
						case 'rgba_dxtc1':
						case 'rgba_dxtc3':
						case 'rgba_dxtc3_p':
						case 'rgba_dxtc5':
						case 'rgba_dxtc5_p': {
							channel = format.split('_')[0].split('');
							break;
						}
						case 'rgb_pvrtc1_4bpp':
						case 'rgba_pvrtc1_4bpp': {
							channel = format.split('_')[0].split('');
							break;
						}
						case 'rgb_etc1':
						case 'rgb_etc2':
						case 'rgba_etc2_eac':
						case 'r_eac':
						case 'rg_eac': {
							channel = format.split('_')[0].split('');
							break;
						}
						case 'rgba_astc_4x4':
						case 'rgba_astc_5x4':
						case 'rgba_astc_5x5':
						case 'rgba_astc_6x5':
						case 'rgba_astc_6x6':
						case 'rgba_astc_8x5':
						case 'rgba_astc_8x6':
						case 'rgba_astc_8x8':
						case 'rgba_astc_10x5':
						case 'rgba_astc_10x6':
						case 'rgba_astc_10x8':
						case 'rgba_astc_10x10':
						case 'rgba_astc_12x10':
						case 'rgba_astc_12x12': {
							channel = format.split('_')[0].split('');
							break;
						}
					}
					return [
						channel.includes('r') ? null : 255n,
						channel.includes('g') ? null : 255n,
						channel.includes('b') ? null : 255n,
						channel.includes('a') ? null : 255n,
					];
				}

				export function get_block_size(
					format: CompositeFormat,
				): Image.ImageSize {
					let result: Image.ImageSize;
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
						case 'rgb_888_r':
						case 'rgba_8888_o':
						case 'rgba_8888_r':
						case 'argb_8888_o':
						case 'argb_8888_r': {
							result = [1n, 1n];
							break;
						}
						case 'rgba_dxtc1':
						case 'rgba_dxtc3':
						case 'rgba_dxtc3_p':
						case 'rgba_dxtc5':
						case 'rgba_dxtc5_p': {
							result = [4n, 4n];
							break;
						}
						case 'rgb_pvrtc1_4bpp':
						case 'rgba_pvrtc1_4bpp': {
							result = [4n, 4n];
							break;
						}
						case 'rgb_etc1':
						case 'rgb_etc2':
						case 'rgba_etc2_eac':
						case 'r_eac':
						case 'rg_eac': {
							result = [4n, 4n];
							break;
						}
						case 'rgba_astc_4x4':
						case 'rgba_astc_5x4':
						case 'rgba_astc_5x5':
						case 'rgba_astc_6x5':
						case 'rgba_astc_6x6':
						case 'rgba_astc_8x5':
						case 'rgba_astc_8x6':
						case 'rgba_astc_8x8':
						case 'rgba_astc_10x5':
						case 'rgba_astc_10x6':
						case 'rgba_astc_10x8':
						case 'rgba_astc_10x10':
						case 'rgba_astc_12x10':
						case 'rgba_astc_12x12': {
							let block_size_expression = format.substring('rgba_astc_'.length).split('x');
							result = [BigInt(block_size_expression[0]), BigInt(block_size_expression[1])];
							break;
						}
					}
					return result;
				}

				export function get_block_bit_count(
					format: CompositeFormat,
				): bigint {
					let result: bigint;
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
						case 'rgb_888_r':
						case 'rgba_8888_o':
						case 'rgba_8888_r':
						case 'argb_8888_o':
						case 'argb_8888_r': {
							let channel_bit_count = format.split('_')[1].split('').map(BigInt);
							result = channel_bit_count.reduce((a, b) => a + b, 0n);
							break;
						}
						case 'rgba_dxtc1':
						case 'rgba_dxtc3':
						case 'rgba_dxtc3_p':
						case 'rgba_dxtc5':
						case 'rgba_dxtc5_p': {
							result = 64n;
							if (format !== 'rgba_dxtc1') {
								result += 64n;
							}
							break;
						}
						case 'rgb_pvrtc1_4bpp':
						case 'rgba_pvrtc1_4bpp': {
							result = 64n;
							break;
						}
						case 'rgb_etc1':
						case 'rgb_etc2':
						case 'rgba_etc2_eac':
						case 'r_eac':
						case 'rg_eac': {
							result = 64n;
							if (format === 'rgba_etc2_eac' || format === 'rg_eac') {
								result += 64n;
							}
							break;
						}
						case 'rgba_astc_4x4':
						case 'rgba_astc_5x4':
						case 'rgba_astc_5x5':
						case 'rgba_astc_6x5':
						case 'rgba_astc_6x6':
						case 'rgba_astc_8x5':
						case 'rgba_astc_8x6':
						case 'rgba_astc_8x8':
						case 'rgba_astc_10x5':
						case 'rgba_astc_10x6':
						case 'rgba_astc_10x8':
						case 'rgba_astc_10x10':
						case 'rgba_astc_12x10':
						case 'rgba_astc_12x12': {
							result = 128n;
							break;
						}
					}
					return result;
				}

				export function compute_data_size(
					size: Image.ImageSize,
					format: CompositeFormat,
				): bigint {
					let block_size = get_block_size(format);
					let block_bit_count = get_block_bit_count(format);
					return (size[0] * size[1]) / (block_size[0] * block_size[1]) * block_bit_count / 8n;
				}

				export function compute_padded_image_size(
					origin_size: Image.ImageSize,
					format: CompositeFormat,
				): Image.ImageSize {
					let block_size = get_block_size(format);
					let padded_size: Image.ImageSize = [
						ConvertHelper.compute_padded_size(origin_size[0], block_size[0]),
						ConvertHelper.compute_padded_size(origin_size[1], block_size[1]),
					];
					if (format.includes('etc') || format.includes('pvrtc')) {
						padded_size = [
							ConvertHelper.compute_padded_size_of_exponent_of_2(padded_size[0]),
							ConvertHelper.compute_padded_size_of_exponent_of_2(padded_size[1]),
						];
					}
					if (format.includes('pvrtc')) {
						let maximum_size = padded_size[0] > padded_size[1] ? padded_size[0] : padded_size[1];
						padded_size = [maximum_size, maximum_size];
					}
					return padded_size;
				}

				// ----------------

				export function encode(
					data: Kernel.OutputByteStreamView,
					image: Kernel.Image.ConstantImageView,
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
						case 'rgb_888_r':
						case 'rgba_8888_o':
						case 'rgba_8888_r':
						case 'argb_8888_o':
						case 'argb_8888_r': {
							Kernel.Tool.Texture.Encoding.Encode.process(
								data,
								image,
								Kernel.Tool.Texture.Encoding.Format.value(format),
							);
							break;
						}
						case 'rgba_dxtc1':
						case 'rgba_dxtc3':
						case 'rgba_dxtc3_p':
						case 'rgba_dxtc5':
						case 'rgba_dxtc5_p': {
							let generation = ({
								'rgba_dxtc1': 'v1',
								'rgba_dxtc3': 'v3',
								'rgba_dxtc3_p': 'v3',
								'rgba_dxtc5': 'v5',
								'rgba_dxtc5_p': 'v5',
							} as Record<typeof format, typeof Kernel.Tool.Texture.Compression.Dxtc.Generation.Value>)[format];
							let premultiplying = format.endsWith('_p');
							if (!premultiplying) {
								Kernel.Tool.Texture.Compression.Dxtc.Compress.process(
									data,
									image,
									Kernel.Tool.Texture.Compression.Dxtc.Generation.value(generation),
									Kernel.Image.ImageSize.value(get_block_size(format)),
									Kernel.Boolean.value(true),
								);
							}
							else {
								let premultiplying_image = Kernel.Image.Image.allocate(image.size());
								let premultiplying_image_view = premultiplying_image.view();
								Kernel.Tool.Texture.Transformation.Premultiplying.Encode.process(
									image,
									premultiplying_image_view,
								);
								Kernel.Tool.Texture.Compression.Dxtc.Compress.process(
									data,
									premultiplying_image_view,
									Kernel.Tool.Texture.Compression.Dxtc.Generation.value(generation),
									Kernel.Image.ImageSize.value(get_block_size(format)),
									Kernel.Boolean.value(true),
								);
							}
							break;
						}
						case 'rgb_pvrtc1_4bpp':
						case 'rgba_pvrtc1_4bpp': {
							let generation = ({
								'rgb_pvrtc1_4bpp': 'v1',
								'rgba_pvrtc1_4bpp': 'v1',
							} as Record<typeof format, typeof Kernel.Tool.Texture.Compression.Pvrtc.Generation.Value>)[format];
							let with_alpha = format === 'rgba_pvrtc1_4bpp';
							Kernel.Tool.Texture.Compression.Pvrtc.Compress.process(
								data,
								image,
								Kernel.Tool.Texture.Compression.Pvrtc.Generation.value(generation),
								Kernel.Image.ImageSize.value(get_block_size(format)),
								Kernel.Boolean.value(true),
								Kernel.Boolean.value(with_alpha),
							);
							break;
						}
						case 'rgb_etc1':
						case 'rgb_etc2':
						case 'rgba_etc2_eac':
						case 'r_eac':
						case 'rg_eac': {
							let generation = ({
								'rgb_etc1': 'v1',
								'rgb_etc2': 'v2',
								'rgba_etc2_eac': 'v2',
								'r_eac': 'eac',
								'rg_eac': 'eac',
							} as Record<typeof format, typeof Kernel.Tool.Texture.Compression.Etc.Generation.Value>)[format];
							let with_alpha_eac = format === 'rgba_etc2_eac';
							let with_green_eac = format === 'rg_eac';
							Kernel.Tool.Texture.Compression.Etc.Compress.process(
								data,
								image,
								Kernel.Tool.Texture.Compression.Etc.Generation.value(generation),
								Kernel.Image.ImageSize.value(get_block_size(format)),
								Kernel.Boolean.value(with_alpha_eac),
								Kernel.Boolean.value(with_green_eac),
							);
							break;
						}
						case 'rgba_astc_4x4':
						case 'rgba_astc_5x4':
						case 'rgba_astc_5x5':
						case 'rgba_astc_6x5':
						case 'rgba_astc_6x6':
						case 'rgba_astc_8x5':
						case 'rgba_astc_8x6':
						case 'rgba_astc_8x8':
						case 'rgba_astc_10x5':
						case 'rgba_astc_10x6':
						case 'rgba_astc_10x8':
						case 'rgba_astc_10x10':
						case 'rgba_astc_12x10':
						case 'rgba_astc_12x12': {
							let generation = 'v0' as typeof Kernel.Tool.Texture.Compression.Astc.Generation.Value;
							Kernel.Tool.Texture.Compression.Astc.Compress.process(
								data,
								image,
								Kernel.Tool.Texture.Compression.Astc.Generation.value(generation),
								Kernel.Image.ImageSize.value(get_block_size(format)),
								Kernel.Integer.value(60n),
							);
							break;
						}
					}
					return;
				}

				export function decode(
					data: Kernel.InputByteStreamView,
					image: Kernel.Image.VariableImageView,
					format: CompositeFormat,
					fill_dummy: Boolean,
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
							Kernel.Tool.Texture.Encoding.Decode.process(
								data,
								image,
								Kernel.Tool.Texture.Encoding.Format.value(format),
							);
							break;
						}
						case 'rgba_dxtc1':
						case 'rgba_dxtc3':
						case 'rgba_dxtc3_p':
						case 'rgba_dxtc5':
						case 'rgba_dxtc5_p': {
							let generation = ({
								'rgba_dxtc1': 'v1',
								'rgba_dxtc3': 'v3',
								'rgba_dxtc3_p': 'v3',
								'rgba_dxtc5': 'v5',
								'rgba_dxtc5_p': 'v5',
							} as Record<typeof format, typeof Kernel.Tool.Texture.Compression.Dxtc.Generation.Value>)[format];
							let premultiplying = format.endsWith('_p');
							if (!premultiplying) {
								Kernel.Tool.Texture.Compression.Dxtc.Uncompress.process(
									data,
									image,
									Kernel.Tool.Texture.Compression.Dxtc.Generation.value(generation),
									Kernel.Image.ImageSize.value(get_block_size(format)),
									Kernel.Boolean.value(true),
								);
							}
							else {
								let premultiplying_image = Kernel.Image.Image.allocate(image.size());
								let premultiplying_image_view = premultiplying_image.view();
								Kernel.Tool.Texture.Compression.Dxtc.Uncompress.process(
									data,
									premultiplying_image_view,
									Kernel.Tool.Texture.Compression.Dxtc.Generation.value(generation),
									Kernel.Image.ImageSize.value(get_block_size(format)),
									Kernel.Boolean.value(true),
								);
								Kernel.Tool.Texture.Transformation.Premultiplying.Decode.process(
									image,
									premultiplying_image_view,
								);
							}
							break;
						}
						case 'rgb_pvrtc1_4bpp':
						case 'rgba_pvrtc1_4bpp': {
							let generation = ({
								'rgb_pvrtc1_4bpp': 'v1',
								'rgba_pvrtc1_4bpp': 'v1',
							} as Record<typeof format, typeof Kernel.Tool.Texture.Compression.Pvrtc.Generation.Value>)[format];
							let with_alpha = format === 'rgba_pvrtc1_4bpp';
							Kernel.Tool.Texture.Compression.Pvrtc.Uncompress.process(
								data,
								image,
								Kernel.Tool.Texture.Compression.Pvrtc.Generation.value(generation),
								Kernel.Image.ImageSize.value(get_block_size(format)),
								Kernel.Boolean.value(true),
								Kernel.Boolean.value(with_alpha),
							);
							break;
						}
						case 'rgb_etc1':
						case 'rgb_etc2':
						case 'rgba_etc2_eac':
						case 'r_eac':
						case 'rg_eac': {
							let generation = ({
								'rgb_etc1': 'v1',
								'rgb_etc2': 'v2',
								'rgba_etc2_eac': 'v2',
								'r_eac': 'eac',
								'rg_eac': 'eac',
							} as Record<typeof format, typeof Kernel.Tool.Texture.Compression.Etc.Generation.Value>)[format];
							let with_alpha_eac = format === 'rgba_etc2_eac';
							let with_green_eac = format === 'rg_eac';
							Kernel.Tool.Texture.Compression.Etc.Uncompress.process(
								data,
								image,
								Kernel.Tool.Texture.Compression.Etc.Generation.value(generation),
								Kernel.Image.ImageSize.value(get_block_size(format)),
								Kernel.Boolean.value(with_alpha_eac),
								Kernel.Boolean.value(with_green_eac),
							);
							break;
						}
						case 'rgba_astc_4x4':
						case 'rgba_astc_5x4':
						case 'rgba_astc_5x5':
						case 'rgba_astc_6x5':
						case 'rgba_astc_6x6':
						case 'rgba_astc_8x5':
						case 'rgba_astc_8x6':
						case 'rgba_astc_8x8':
						case 'rgba_astc_10x5':
						case 'rgba_astc_10x6':
						case 'rgba_astc_10x8':
						case 'rgba_astc_10x10':
						case 'rgba_astc_12x10':
						case 'rgba_astc_12x12': {
							let generation = 'v0' as typeof Kernel.Tool.Texture.Compression.Astc.Generation.Value;
							Kernel.Tool.Texture.Compression.Astc.Uncompress.process(
								data,
								image,
								Kernel.Tool.Texture.Compression.Astc.Generation.value(generation),
								Kernel.Image.ImageSize.value(get_block_size(format)),
							);
							break;
						}
					}
					if (fill_dummy) {
						let dummy_channel_filler = get_dummy_channel_filler(format);
						if (dummy_channel_filler.some((it) => it !== null)) {
							let filled_image = Kernel.Image.Image.allocate(image.size());
							let filled_image_view = filled_image.view();
							Transformation.Filling.encode(filled_image_view, image, dummy_channel_filler[0], dummy_channel_filler[1], dummy_channel_filler[2], dummy_channel_filler[3]);
							image.draw(filled_image_view);
						}
					}
					return;
				}

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					image_file: StoragePath,
					format: CompositeFormat,
				): void {
					let image_original = StorageHelper.read_file(image_file);
					let image_stream = Kernel.ByteStreamView.watch(image_original.view());
					let image_size = Conversion.Png.size(image_stream.view());
					let image_size_padded = compute_padded_image_size(image_size, format);
					let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(image_size_padded));
					let image_view = image.view();
					Conversion.Png.decode(image_stream, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
					let data_size = compute_data_size(image_size_padded, format);
					let data = Kernel.ByteArray.allocate(Kernel.Size.value(data_size));
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					encode(data_stream, image_view, format);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					image_file: StoragePath,
					image_size: Image.ImageSize,
					format: CompositeFormat,
				): void {
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image_size_padded = compute_padded_image_size(image_size, format);
					let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(image_size_padded));
					let image_view = image.view();
					decode(data_stream, image_view, format, true);
					Conversion.Png.encode_fs(image_file, image_view.sub(Kernel.Image.ImagePosition.value([0n, 0n]), Kernel.Image.ImageSize.value(image_size)));
					return;
				}

			}

			export namespace Conversion {

				export namespace Png {

					export function size(
						data: Kernel.ByteListView,
					): Image.ImageSize {
						let size = Kernel.Image.ImageSize.default();
						Kernel.Tool.Texture.Conversion.Png.Decode.estimate(data, size);
						return size.value;
					}

					export function encode(
						data: Kernel.ByteStreamView,
						image: Kernel.Image.ConstantImageView,
					): void {
						return Kernel.Tool.Texture.Conversion.Png.Encode.process(data, image);
					}

					export function decode(
						data: Kernel.ByteStreamView,
						image: Kernel.Image.VariableImageView,
					): void {
						return Kernel.Tool.Texture.Conversion.Png.Decode.process(data, image);
					}

					// ----------------

					export function size_fs(
						file: StoragePath,
					): Image.ImageSize {
						let data = StorageHelper.read_file(file);
						let image_size = size(data.view());
						return image_size;
					}

					export function encode_fs(
						file: StoragePath,
						image: Kernel.Image.ConstantImageView,
						data_buffer: Kernel.ByteListView = g_common_buffer.view(),
					): void {
						let data_stream = Kernel.ByteStreamView.watch(data_buffer);
						encode(data_stream, image);
						StorageHelper.write_file(file, data_stream.stream_view());
						return;
					}

					export function decode_fs(
						file: StoragePath,
						image: Kernel.Image.VariableImageView,
					): void {
						let data = StorageHelper.read_file(file);
						let data_stream = Kernel.ByteStreamView.watch(data.view());
						decode(data_stream, image);
						return;
					}

					// ----------------

					export function decode_fs_of(
						file: StoragePath,
					): Kernel.Image.Image {
						let data = StorageHelper.read_file(file);
						let data_stream = Kernel.ByteStreamView.watch(data.view());
						let image_size = size(data.view());
						let image = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(image_size));
						decode(data_stream, image.view());
						return image;
					}

				}

			}

		}

		export namespace Wwise {

			export namespace SoundBank {

				export type VersionNumber = typeof Kernel.Tool.Wwise.SoundBank.Version.Value.number;

				export const VersionNumberE = [72n, 88n, 112n, 113n, 118n, 120n, 125n, 128n, 132n, 134n, 135n, 140n, 145n, 150n] as VersionNumber[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					embedded_media_directory: StoragePath,
					version: typeof Kernel.Tool.Wwise.SoundBank.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Wwise.SoundBank.Version.value(version);
					let definition = Kernel.Tool.Wwise.SoundBank.Definition.SoundBank.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Wwise.SoundBank.Encode.process(data_stream, definition, Kernel.Path.value(embedded_media_directory.emit()), version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: null | StoragePath,
					embedded_media_directory: null | StoragePath,
					version: typeof Kernel.Tool.Wwise.SoundBank.Version.Value,
				): void {
					let version_c = Kernel.Tool.Wwise.SoundBank.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Wwise.SoundBank.Definition.SoundBank.default();
					Kernel.Tool.Wwise.SoundBank.Decode.process(data_stream, definition, Kernel.PathOptional.value(embedded_media_directory === null ? null : embedded_media_directory.emit()), version_c);
					if (definition_file !== null) {
						Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					}
					return;
				}

			}

		}

		export namespace Marmalade {

			export namespace Dzip {

				export type VersionNumber = typeof Kernel.Tool.Marmalade.Dzip.Version.Value.number;

				export const VersionNumberE = [0n] as VersionNumber[];

				// ----------------

				export function pack_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					resource_directory: StoragePath,
					version: typeof Kernel.Tool.Marmalade.Dzip.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Marmalade.Dzip.Version.value(version);
					let definition = Kernel.Tool.Marmalade.Dzip.Definition.Package.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Marmalade.Dzip.Pack.process(data_stream, definition, Kernel.Path.value(resource_directory.emit()), version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: StoragePath,
					definition_file: null | StoragePath,
					resource_directory: null | StoragePath,
					version: typeof Kernel.Tool.Marmalade.Dzip.Version.Value,
				): void {
					let version_c = Kernel.Tool.Marmalade.Dzip.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Marmalade.Dzip.Definition.Package.default();
					Kernel.Tool.Marmalade.Dzip.Unpack.process(data_stream, definition, Kernel.PathOptional.value(resource_directory === null ? null : resource_directory.emit()), version_c);
					if (definition_file !== null) {
						Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					}
					return;
				}

			}

		}

		export namespace Popcap {

			export namespace Zlib {

				export type VersionVariant64 = typeof Kernel.Tool.Popcap.Zlib.Version.Value.variant_64;

				export const VersionVariant64E = [false, true] as VersionVariant64[];

				// ----------------

				export function compress_fs(
					raw_file: StoragePath,
					ripe_file: StoragePath,
					level: Data.Compression.Deflate.Level,
					window_exponent: Data.Compression.Deflate.WindowExponent,
					memory_level: Data.Compression.Deflate.MemoryLevel,
					strategy_mode: Data.Compression.Deflate.StrategyMode,
					version: typeof Kernel.Tool.Popcap.Zlib.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.Zlib.Version.value(version);
					let raw = StorageHelper.read_file(raw_file);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					let ripe_size_bound = Kernel.Size.default();
					Kernel.Tool.Popcap.Zlib.Compress.estimate(raw.size(), ripe_size_bound, Kernel.Integer.value(window_exponent), Kernel.Integer.value(memory_level), version_c);
					let ripe = Kernel.ByteArray.allocate(ripe_size_bound);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					Kernel.Tool.Popcap.Zlib.Compress.process(raw_stream, ripe_stream, Kernel.Integer.value(level), Kernel.Integer.value(window_exponent), Kernel.Integer.value(memory_level), Kernel.Tool.Data.Compression.Deflate.StrategyMode.value(strategy_mode), version_c);
					StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
					return;
				}

				export function uncompress_fs(
					raw_file: StoragePath,
					ripe_file: StoragePath,
					window_exponent: Data.Compression.Deflate.WindowExponent,
					version: typeof Kernel.Tool.Popcap.Zlib.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.Zlib.Version.value(version);
					let ripe = StorageHelper.read_file(ripe_file);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					let raw_size = Kernel.Size.default();
					Kernel.Tool.Popcap.Zlib.Uncompress.estimate(raw_size, ripe.view(), version_c);
					let raw = Kernel.ByteArray.allocate(raw_size);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					Kernel.Tool.Popcap.Zlib.Uncompress.process(raw_stream, ripe_stream, Kernel.Integer.value(window_exponent), version_c);
					StorageHelper.write_file(raw_file, raw_stream.stream_view());
					return;
				}

			}

			export namespace CryptData {

				export function encrypt_fs(
					raw_file: StoragePath,
					ripe_file: StoragePath,
					limit: bigint,
					key: string,
					version: typeof Kernel.Tool.Popcap.CryptData.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.CryptData.Version.value(version);
					let raw = StorageHelper.read_file(raw_file);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					let ripe_size = Kernel.Size.default();
					Kernel.Tool.Popcap.CryptData.Encrypt.estimate(raw.size(), ripe_size, Kernel.Size.value(limit), version_c);
					let ripe = Kernel.ByteArray.allocate(ripe_size);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					Kernel.Tool.Popcap.CryptData.Encrypt.process(raw_stream, ripe_stream, Kernel.Size.value(limit), Kernel.String.value(key), version_c);
					StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
					return;
				}

				export function decrypt_fs(
					raw_file: StoragePath,
					ripe_file: StoragePath,
					limit: bigint,
					key: string,
					version: typeof Kernel.Tool.Popcap.CryptData.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.CryptData.Version.value(version);
					let ripe = StorageHelper.read_file(ripe_file);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					let raw_size = Kernel.Size.default();
					Kernel.Tool.Popcap.CryptData.Decrypt.estimate(raw_size, ripe.view(), Kernel.Size.value(limit), version_c);
					let raw = Kernel.ByteArray.allocate(raw_size);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					Kernel.Tool.Popcap.CryptData.Decrypt.process(raw_stream, ripe_stream, Kernel.Size.value(limit), Kernel.String.value(key), version_c);
					StorageHelper.write_file(raw_file, raw_stream.stream_view());
					return;
				}

			}

			export namespace ReflectionObjectNotation {

				export type VersionNumber = typeof Kernel.Tool.Popcap.ReflectionObjectNotation.Version.Value.number;

				export const VersionNumberE = [1n] as VersionNumber[];

				export type VersionNativeStringEncodingUseUtf8 = typeof Kernel.Tool.Popcap.ReflectionObjectNotation.Version.Value.native_string_encoding_use_utf8;

				export const VersionNativeStringEncodingUseUtf8E = [false, true] as VersionNativeStringEncodingUseUtf8[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					enable_string_index: boolean,
					enable_reference: boolean,
					version: typeof Kernel.Tool.Popcap.ReflectionObjectNotation.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.ReflectionObjectNotation.Version.value(version);
					let definition = Data.Serialization.Json.decode_fs<Kernel.Tool.Popcap.ReflectionObjectNotation.JS_ValidValue>(definition_file);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.ReflectionObjectNotation.Encode.process(data_stream, definition, Kernel.Boolean.value(enable_string_index), Kernel.Boolean.value(enable_reference), version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.ReflectionObjectNotation.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.ReflectionObjectNotation.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Notation.Json.Value.default<Kernel.Tool.Popcap.ReflectionObjectNotation.JS_ValidValue>();
					Kernel.Tool.Popcap.ReflectionObjectNotation.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition);
					return;
				}

				export function encode_cipher_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					enable_string_index: boolean,
					enable_reference: boolean,
					version: typeof Kernel.Tool.Popcap.ReflectionObjectNotation.Version.Value,
					key: null | string,
					data_buffer: Kernel.ByteListView,
				): void {
					if (key === null) {
						return encode_fs(data_file, definition_file, enable_string_index, enable_reference, version, data_buffer);
					}
					let version_c = Kernel.Tool.Popcap.ReflectionObjectNotation.Version.value(version);
					let definition = Data.Serialization.Json.decode_fs<Kernel.Tool.Popcap.ReflectionObjectNotation.JS_ValidValue>(definition_file);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.ReflectionObjectNotation.Encode.process(data_stream, definition, Kernel.Boolean.value(enable_string_index), Kernel.Boolean.value(enable_reference), version_c);
					let raw_stream = Kernel.ByteStreamView.watch(data_stream.stream_view());
					let ripe_size = Kernel.Size.default();
					Kernel.Tool.Miscellaneous.Pvz2cnCryptData.Encrypt.estimate(raw_stream.size(), ripe_size);
					let ripe = Kernel.ByteArray.allocate(ripe_size);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					Kernel.Tool.Miscellaneous.Pvz2cnCryptData.Encrypt.process(raw_stream, ripe_stream, Kernel.String.value(key));
					StorageHelper.write_file(data_file, ripe_stream.stream_view());
					return;
				}

				export function decode_cipher_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.ReflectionObjectNotation.Version.Value,
					key: null | string,
				): void {
					if (key === null) {
						return decode_fs(data_file, definition_file, version);
					}
					let version_c = Kernel.Tool.Popcap.ReflectionObjectNotation.Version.value(version);
					let ripe = StorageHelper.read_file(data_file);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					let raw_size = Kernel.Size.default();
					Kernel.Tool.Miscellaneous.Pvz2cnCryptData.Decrypt.estimate(raw_size, ripe.size());
					let raw = Kernel.ByteArray.allocate(raw_size);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					Kernel.Tool.Miscellaneous.Pvz2cnCryptData.Decrypt.process(raw_stream, ripe_stream, Kernel.String.value(key));
					let data_stream = Kernel.ByteStreamView.watch(raw_stream.stream_view());
					let definition = Kernel.Notation.Json.Value.default<Kernel.Tool.Popcap.ReflectionObjectNotation.JS_ValidValue>();
					Kernel.Tool.Popcap.ReflectionObjectNotation.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition);
					return;
				}

			}

			export namespace UTexture {

				export type VersionCompressTextureData = typeof Kernel.Tool.Popcap.UTexture.Version.Value.compress_texture_data;

				export const VersionCompressTextureDataE = [false, true] as VersionCompressTextureData[];

				const FormatX = [
					'rgba_8888_o',
					'rgba_4444',
					'rgba_5551',
					'rgb_565',
				] as const;

				export type Format = typeof FormatX[number];

				export const FormatE = FormatX as unknown as Format[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					image_file: StoragePath,
					format: Format,
					version: typeof Kernel.Tool.Popcap.UTexture.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.UTexture.Version.value(version);
					let image = Texture.Conversion.Png.decode_fs_of(image_file);
					let image_view = image.view();
					let data_size_bound = Kernel.Size.default();
					Kernel.Tool.Popcap.UTexture.Encode.estimate(data_size_bound, image.size(), Kernel.Tool.Texture.Encoding.Format.value(format), version_c);
					let data = Kernel.ByteArray.allocate(data_size_bound);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					Kernel.Tool.Popcap.UTexture.Encode.process(data_stream, image_view, Kernel.Tool.Texture.Encoding.Format.value(format), version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					image_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.UTexture.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.UTexture.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image_size = Kernel.Image.ImageSize.default();
					Kernel.Tool.Popcap.UTexture.Decode.estimate(data.view(), image_size, version_c);
					let image = Kernel.Image.Image.allocate(image_size);
					let image_view = image.view();
					Kernel.Tool.Popcap.UTexture.Decode.process(data_stream, image_view, version_c);
					Texture.Conversion.Png.encode_fs(image_file, image_view);
					return;
				}

			}

			export namespace SexyTexture {

				export type VersionNumber = typeof Kernel.Tool.Popcap.SexyTexture.Version.Value.number;

				export const VersionNumberE = [0n] as VersionNumber[];

				const FormatX = [
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

				export type Format = typeof FormatX[number];

				export const FormatE = FormatX as unknown as Format[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					image_file: StoragePath,
					format: Format,
					compress_texture_data: boolean,
					version: typeof Kernel.Tool.Popcap.SexyTexture.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.SexyTexture.Version.value(version);
					let image = Texture.Conversion.Png.decode_fs_of(image_file);
					let image_view = image.view();
					let data_size_bound = Kernel.Size.default();
					Kernel.Tool.Popcap.SexyTexture.Encode.estimate(data_size_bound, image.size(), Kernel.Tool.Texture.Encoding.Format.value(format), Kernel.Boolean.value(compress_texture_data), version_c);
					let data = Kernel.ByteArray.allocate(data_size_bound);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					Kernel.Tool.Popcap.SexyTexture.Encode.process(data_stream, image_view, Kernel.Tool.Texture.Encoding.Format.value(format), Kernel.Boolean.value(compress_texture_data), version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					image_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.SexyTexture.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.SexyTexture.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let image_size = Kernel.Image.ImageSize.default();
					Kernel.Tool.Popcap.SexyTexture.Decode.estimate(data.view(), image_size, version_c);
					let image = Kernel.Image.Image.allocate(image_size);
					let image_view = image.view();
					Kernel.Tool.Popcap.SexyTexture.Decode.process(data_stream, image_view, version_c);
					Texture.Conversion.Png.encode_fs(image_file, image_view);
					return;
				}

			}

			export namespace Animation {

				export type VersionNumber = typeof Kernel.Tool.Popcap.Animation.Version.Value.number;

				export const VersionNumberE = [1n, 2n, 3n, 4n, 5n, 6n] as VersionNumber[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.Animation.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.Animation.Version.value(version);
					let definition = Kernel.Tool.Popcap.Animation.Definition.Animation.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.Animation.Encode.process(data_stream, definition, version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.Animation.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.Animation.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.Animation.Definition.Animation.default();
					Kernel.Tool.Popcap.Animation.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace ReAnimation {

				export type VersionPlatform = typeof Kernel.Tool.Popcap.ReAnimation.Version.Value.platform;

				export const VersionPlatformE = ['desktop', 'mobile', 'television'] as VersionPlatform[];

				export type VersionVariant64 = typeof Kernel.Tool.Popcap.ReAnimation.Version.Value.variant_64;

				export const VersionVariant64E = [false, true] as VersionVariant64[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.ReAnimation.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.ReAnimation.Version.value(version);
					let definition = Kernel.Tool.Popcap.ReAnimation.Definition.Animation.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.ReAnimation.Encode.process(data_stream, definition, version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.ReAnimation.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.ReAnimation.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.ReAnimation.Definition.Animation.default();
					Kernel.Tool.Popcap.ReAnimation.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace Particle {

				export type VersionPlatform = typeof Kernel.Tool.Popcap.Particle.Version.Value.platform;

				export const VersionPlatformE = ['desktop', 'mobile', 'television'] as VersionPlatform[];

				export type VersionVariant64 = typeof Kernel.Tool.Popcap.Particle.Version.Value.variant_64;

				export const VersionVariant64E = [false, true] as VersionVariant64[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.Particle.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.Particle.Version.value(version);
					let definition = Kernel.Tool.Popcap.Particle.Definition.Particle.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.Particle.Encode.process(data_stream, definition, version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.Particle.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.Particle.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.Particle.Definition.Particle.default();
					Kernel.Tool.Popcap.Particle.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace Trail {

				export type VersionPlatform = typeof Kernel.Tool.Popcap.Trail.Version.Value.platform;

				export const VersionPlatformE = ['desktop', 'mobile', 'television'] as VersionPlatform[];

				export type VersionVariant64 = typeof Kernel.Tool.Popcap.Trail.Version.Value.variant_64;

				export const VersionVariant64E = [false, true] as VersionVariant64[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.Trail.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.Trail.Version.value(version);
					let definition = Kernel.Tool.Popcap.Trail.Definition.Trail.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.Trail.Encode.process(data_stream, definition, version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.Trail.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.Trail.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.Trail.Definition.Trail.default();
					Kernel.Tool.Popcap.Trail.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace RenderEffect {

				export type VersionNumber = typeof Kernel.Tool.Popcap.RenderEffect.Version.Value.number;

				export const VersionNumberE = [1n] as VersionNumber[];

				export type VersionVariant = typeof Kernel.Tool.Popcap.RenderEffect.Version.Value.variant;

				export const VersionVariantE = [1n, 2n, 3n] as VersionVariant[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.RenderEffect.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.RenderEffect.Version.value(version);
					let definition = Kernel.Tool.Popcap.RenderEffect.Definition.Effect.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.RenderEffect.Encode.process(data_stream, definition, version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.RenderEffect.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.RenderEffect.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.RenderEffect.Definition.Effect.default();
					Kernel.Tool.Popcap.RenderEffect.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace ParticleEffect {

				export type VersionNumber = typeof Kernel.Tool.Popcap.ParticleEffect.Version.Value.number;

				export const VersionNumberE = [1n] as VersionNumber[];

				// ----------------

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.ParticleEffect.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.ParticleEffect.Version.value(version);
					let definition = Kernel.Tool.Popcap.ParticleEffect.Definition.Effect.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.ParticleEffect.Encode.process(data_stream, definition, version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.ParticleEffect.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.ParticleEffect.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.ParticleEffect.Definition.Effect.default();
					Kernel.Tool.Popcap.ParticleEffect.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace CharacterFontWidget2 {

				export function encode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.CharacterFontWidget2.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.CharacterFontWidget2.Version.value(version);
					let definition = Kernel.Tool.Popcap.CharacterFontWidget2.Definition.FontWidget.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					data_stream.set_position(Kernel.Size.value(16n));
					Kernel.Tool.Popcap.CharacterFontWidget2.Encode.process(data_stream, definition, version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function decode_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					version: typeof Kernel.Tool.Popcap.CharacterFontWidget2.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.CharacterFontWidget2.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.CharacterFontWidget2.Definition.FontWidget.default();
					data_stream.set_position(Kernel.Size.value(16n));
					Kernel.Tool.Popcap.CharacterFontWidget2.Decode.process(data_stream, definition, version_c);
					Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					return;
				}

			}

			export namespace Package {

				export type VersionNumber = typeof Kernel.Tool.Popcap.Package.Version.Value.number;

				export const VersionNumberE = [0n] as VersionNumber[];

				export type VersionCompressResourceData = typeof Kernel.Tool.Popcap.Package.Version.Value.compress_resource_data;

				export const VersionCompressResourceDataE = [false, true] as VersionCompressResourceData[];

				// ----------------

				export function pack_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					resource_directory: StoragePath,
					version: typeof Kernel.Tool.Popcap.Package.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.Package.Version.value(version);
					let definition = Kernel.Tool.Popcap.Package.Definition.Package.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.Package.Pack.process(data_stream, definition, Kernel.Path.value(resource_directory.emit()), version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: StoragePath,
					definition_file: null | StoragePath,
					resource_directory: null | StoragePath,
					version: typeof Kernel.Tool.Popcap.Package.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.Package.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.Package.Definition.Package.default();
					Kernel.Tool.Popcap.Package.Unpack.process(data_stream, definition, Kernel.PathOptional.value(resource_directory === null ? null : resource_directory.emit()), version_c);
					if (definition_file !== null) {
						Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamGroup {

				export type VersionNumber = typeof Kernel.Tool.Popcap.ResourceStreamGroup.Version.Value.number;

				export const VersionNumberE = [1n, 3n, 4n] as VersionNumber[];

				// ----------------

				export function pack_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					resource_directory: StoragePath,
					version: typeof Kernel.Tool.Popcap.ResourceStreamGroup.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.ResourceStreamGroup.Version.value(version);
					let definition = Kernel.Tool.Popcap.ResourceStreamGroup.Definition.Package.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.ResourceStreamGroup.Pack.process(data_stream, definition, Kernel.Path.value(resource_directory.emit()), version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: StoragePath,
					definition_file: null | StoragePath,
					resource_directory: null | StoragePath,
					version: typeof Kernel.Tool.Popcap.ResourceStreamGroup.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.ResourceStreamGroup.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.ResourceStreamGroup.Definition.Package.default();
					Kernel.Tool.Popcap.ResourceStreamGroup.Unpack.process(data_stream, definition, Kernel.PathOptional.value(resource_directory === null ? null : resource_directory.emit()), version_c);
					if (definition_file !== null) {
						Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamBundle {

				export type VersionNumber = typeof Kernel.Tool.Popcap.ResourceStreamBundle.Version.Value.number;

				export const VersionNumberE = [1n, 3n, 4n] as VersionNumber[];

				export type VersionExtendedTextureInformationForPvz2cn = typeof Kernel.Tool.Popcap.ResourceStreamBundle.Version.Value.extended_texture_information_for_pvz2cn;

				export const VersionExtendedTextureInformationForPvz2cnE = [0n, 1n, 2n, 3n] as VersionExtendedTextureInformationForPvz2cn[];

				// ----------------

				export function pack_fs(
					data_file: StoragePath,
					definition_file: StoragePath,
					manifest_file: StoragePath,
					resource_directory: StoragePath,
					packet_file: null | StoragePath,
					new_packet_file: null | StoragePath,
					version: typeof Kernel.Tool.Popcap.ResourceStreamBundle.Version.Value,
					data_buffer: Kernel.ByteListView,
				): void {
					let version_c = Kernel.Tool.Popcap.ResourceStreamBundle.Version.value(version);
					let definition = Kernel.Tool.Popcap.ResourceStreamBundle.Definition.Package.json(Data.Serialization.Json.decode_fs(definition_file), version_c);
					let manifest = Kernel.Tool.Popcap.ResourceStreamBundle.Manifest.PackageOptional.json(Data.Serialization.Json.decode_fs(manifest_file), version_c);
					let data_stream = Kernel.ByteStreamView.watch(data_buffer);
					Kernel.Tool.Popcap.ResourceStreamBundle.Pack.process(data_stream, definition, manifest, Kernel.Path.value(resource_directory.emit()), Kernel.PathOptional.value(packet_file === null ? null : packet_file.emit()), Kernel.PathOptional.value(new_packet_file === null ? null : new_packet_file.emit()), version_c);
					StorageHelper.write_file(data_file, data_stream.stream_view());
					return;
				}

				export function unpack_fs(
					data_file: StoragePath,
					definition_file: null | StoragePath,
					manifest_file: null | StoragePath,
					resource_directory: null | StoragePath,
					packet_file: null | StoragePath,
					version: typeof Kernel.Tool.Popcap.ResourceStreamBundle.Version.Value,
				): void {
					let version_c = Kernel.Tool.Popcap.ResourceStreamBundle.Version.value(version);
					let data = StorageHelper.read_file(data_file);
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					let definition = Kernel.Tool.Popcap.ResourceStreamBundle.Definition.Package.default();
					let manifest = Kernel.Tool.Popcap.ResourceStreamBundle.Manifest.PackageOptional.default();
					Kernel.Tool.Popcap.ResourceStreamBundle.Unpack.process(data_stream, definition, manifest, Kernel.PathOptional.value(resource_directory === null ? null : resource_directory.emit()), Kernel.PathOptional.value(packet_file === null ? null : packet_file.emit()), version_c);
					if (definition_file !== null) {
						Data.Serialization.Json.encode_fs(definition_file, definition.get_json(version_c));
					}
					if (manifest_file !== null) {
						Data.Serialization.Json.encode_fs(manifest_file, manifest.get_json(version_c));
					}
					return;
				}

			}

			export namespace ResourceStreamBundlePatch {

				export type VersionNumber = typeof Kernel.Tool.Popcap.ResourceStreamBundlePatch.Version.Value.number;

				export const VersionNumberE = [1n] as VersionNumber[];

				// ----------------

				export function encode_fs(
					before_file: StoragePath,
					after_file: StoragePath,
					patch_file: StoragePath,
					use_raw_packet: boolean,
					version: typeof Kernel.Tool.Popcap.ResourceStreamBundlePatch.Version.Value,
					patch_size_bound: bigint,
				): void {
					let version_c = Kernel.Tool.Popcap.ResourceStreamBundlePatch.Version.value(version);
					let before = StorageHelper.read_file(before_file);
					let before_stream = Kernel.ByteStreamView.watch(before.view());
					let after = StorageHelper.read_file(after_file);
					let after_stream = Kernel.ByteStreamView.watch(after.view());
					let patch = Kernel.ByteArray.allocate(Kernel.Size.value(patch_size_bound));
					let patch_stream = Kernel.ByteStreamView.watch(patch.view());
					Kernel.Tool.Popcap.ResourceStreamBundlePatch.Encode.process(before_stream, after_stream, patch_stream, Kernel.Boolean.value(use_raw_packet), version_c);
					StorageHelper.write_file(patch_file, patch_stream.stream_view());
					return;
				}

				export function decode_fs(
					before_file: StoragePath,
					after_file: StoragePath,
					patch_file: StoragePath,
					use_raw_packet: boolean,
					version: typeof Kernel.Tool.Popcap.ResourceStreamBundlePatch.Version.Value,
					after_size_bound: bigint,
				): void {
					let version_c = Kernel.Tool.Popcap.ResourceStreamBundlePatch.Version.value(version);
					let before = StorageHelper.read_file(before_file);
					let before_stream = Kernel.ByteStreamView.watch(before.view());
					let after = Kernel.ByteArray.allocate(Kernel.Size.value(after_size_bound));
					let after_stream = Kernel.ByteStreamView.watch(after.view());
					let patch = StorageHelper.read_file(patch_file);
					let patch_stream = Kernel.ByteStreamView.watch(patch.view());
					Kernel.Tool.Popcap.ResourceStreamBundlePatch.Decode.process(before_stream, after_stream, patch_stream, Kernel.Boolean.value(use_raw_packet), version_c);
					StorageHelper.write_file(after_file, after_stream.stream_view());
					return;
				}

			}

		}

		export namespace Miscellaneous {

			export namespace Pvz2cnCryptData {

				export function encrypt_fs(
					raw_file: StoragePath,
					ripe_file: StoragePath,
					key: string,
				): void {
					let raw = StorageHelper.read_file(raw_file);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					let ripe_size = Kernel.Size.default();
					Kernel.Tool.Miscellaneous.Pvz2cnCryptData.Encrypt.estimate(raw.size(), ripe_size);
					let ripe = Kernel.ByteArray.allocate(ripe_size);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					Kernel.Tool.Miscellaneous.Pvz2cnCryptData.Encrypt.process(raw_stream, ripe_stream, Kernel.String.value(key));
					StorageHelper.write_file(ripe_file, ripe_stream.stream_view());
					return;
				}

				export function decrypt_fs(
					raw_file: StoragePath,
					ripe_file: StoragePath,
					key: string,
				): void {
					let ripe = StorageHelper.read_file(ripe_file);
					let ripe_stream = Kernel.ByteStreamView.watch(ripe.view());
					let raw_size = Kernel.Size.default();
					Kernel.Tool.Miscellaneous.Pvz2cnCryptData.Decrypt.estimate(raw_size, ripe.size());
					let raw = Kernel.ByteArray.allocate(raw_size);
					let raw_stream = Kernel.ByteStreamView.watch(raw.view());
					Kernel.Tool.Miscellaneous.Pvz2cnCryptData.Decrypt.process(raw_stream, ripe_stream, Kernel.String.value(key));
					StorageHelper.write_file(raw_file, raw_stream.stream_view());
					return;
				}

			}

			export namespace Pvz2cnAlphaPaletteTexture {

				export type BitCount = 1 | 2 | 3 | 4;

				export function compute_bit_count(
					value_count: number,
				): BitCount {
					assert_test(0b1 < value_count && value_count <= 0b1 << 4);
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

				// ----------------

				export function compute_data_size_with_palette(
					size: Image.ImageSize,
					index_count: number,
				): bigint {
					let bit_count = compute_bit_count(index_count);
					return 1n + BigInt(index_count === 2 ? 0 : index_count) + size[0] * size[1] * BigInt(bit_count) / 8n;
				}

				export function evaluate_palette(
					image: Kernel.Image.ConstantImageView,
				): typeof Kernel.Image.ColorList.Value {
					let image_size = image.size().value;
					let data = Kernel.ByteArray.allocate(Kernel.Size.value(image_size[0] * image_size[1] * 8n / 8n));
					let data_stream = Kernel.ByteStreamView.watch(data.view());
					Texture.Encoding.encode(data_stream, image, 'a_8');
					let alpha_count: Record<number, number> = {};
					for (let value of new Uint8Array(data_stream.stream_view().value)) {
						let alpha_4 = (value >> 4) & ~(~0 << 4);
						alpha_count[alpha_4] = CheckHelper.not_undefined_or(alpha_count[alpha_4], 0) + 1;
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

				// ----------------

				export function encode_with_palette(
					data: Kernel.OutputByteStreamView,
					image: Kernel.Image.ConstantImageView,
					palette: Image.ColorList,
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
					Kernel.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.Encode.process(data, image, Kernel.Image.ColorList.value(palette));
					return;
				}

				export function decode_with_palette(
					data: Kernel.InputByteStreamView,
					image: Kernel.Image.VariableImageView,
				): void {
					let palette_data = new ByteStreamView(data.view().value, Number(data.position().value));
					let index_count = palette_data.u8();
					let palette: Image.ColorList;
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
					Kernel.Tool.Miscellaneous.Pvz2cnAlphaPaletteTexture.Decode.process(data, image, Kernel.Image.ColorList.value(palette));
					return;
				}

			}

		}

	}

	// ----------------

	export namespace Miscellaneous {

		export function evaluate(
			script: string,
			name: string,
			is_module: boolean,
		): any {
			let script_c = Kernel.String.value(script);
			return Kernel.Miscellaneous.g_context.evaluate(Kernel.Miscellaneous.cast_String_to_CharacterListView(script_c), Kernel.String.value(name), Kernel.Boolean.value(is_module));
		}

		export function evaluate_fs(
			script_file: StoragePath,
			is_module: boolean,
		): any {
			let script = StorageHelper.read_file(script_file);
			return Kernel.Miscellaneous.g_context.evaluate(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Kernel.String.value(script_file.emit()), Kernel.Boolean.value(is_module));
		}

		// ----------------

		export function callback(
			argument: Array<string>,
		): Array<string> {
			return Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(argument)).value;
		}

	}

	// #endregion

}
