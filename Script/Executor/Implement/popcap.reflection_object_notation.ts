namespace TwinStar.Script.Executor.Implement.popcap.reflection_object_notation {

	// ------------------------------------------------

	// encode *
	// decode *
	// encrypt *
	// decrypt *
	// encode_then_encrypt *
	// decrypt_then_decode *
	// decode_lenient *

	export type Configuration = {
		version_number: TypicalArgumentExpression<bigint>;
		version_native_string_encoding_use_utf8: TypicalArgumentExpression<boolean>;
		key: TypicalArgumentExpression<string>;
		encode_buffer_size: TypicalArgumentExpression<string>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('popcap.reflection_object_notation', [
			typical_method({
				id: 'encode',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.json)?$/i, '.rton')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_confirmation({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_native_string_encoding_use_utf8,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.encode_buffer_size,
					}),
				],
				worker: ({ definition_file, data_file, version_number, version_native_string_encoding_use_utf8, buffer_size }) => {
					KernelX.Tool.PopCap.ReflectionObjectNotation.encode_fs(data_file, definition_file, true, true, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, buffer_size);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.json)?$/i, '.rton')),
					}),
				],
				batch_worker: ({ definition_file, data_file, version_number, version_native_string_encoding_use_utf8, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.PopCap.ReflectionObjectNotation.encode_fs(data_file, definition_file, true, true, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_confirmation({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_native_string_encoding_use_utf8,
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8 }) => {
					KernelX.Tool.PopCap.ReflectionObjectNotation.decode_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 });
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'encrypt',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						id: 'plain_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'cipher_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/(\.rton)?$/i, '.cipher.rton')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.key,
					}),
				],
				worker: ({ plain_file, cipher_file, key }) => {
					KernelX.Tool.PopCap.ReflectionObjectNotation.encrypt_fs(plain_file, cipher_file, key);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'plain_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'cipher_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file + '.encrypt'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.rton')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'decrypt',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						id: 'cipher_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'plain_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file.replace(/(\.rton)?$/i, '.plain.rton')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ cipher_file, plain_file, key }) => {
					KernelX.Tool.PopCap.ReflectionObjectNotation.decrypt_fs(cipher_file, plain_file, key);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'cipher_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'plain_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file + '.decrypt'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.rton')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'encode_then_encrypt',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.json)?$/i, '.rton')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_confirmation({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_native_string_encoding_use_utf8,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.key,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.encode_buffer_size,
					}),
				],
				worker: ({ definition_file, data_file, version_number, version_native_string_encoding_use_utf8, key, buffer_size }) => {
					KernelX.Tool.PopCap.ReflectionObjectNotation.encode_then_encrypt_fs(data_file, definition_file, true, true, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, key, buffer_size);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode_then_encrypt'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.json)?$/i, '.rton')),
					}),
				],
				batch_worker: ({ definition_file, data_file, version_number, version_native_string_encoding_use_utf8, key, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.PopCap.ReflectionObjectNotation.encode_then_encrypt_fs(data_file, definition_file, true, true, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, key, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'decrypt_then_decode',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_confirmation({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_native_string_encoding_use_utf8,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.key,
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8, key }) => {
					KernelX.Tool.PopCap.ReflectionObjectNotation.decrypt_then_decode_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, key);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decrypt_then_decode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'decode_lenient',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_number,
					}),
					typical_argument_confirmation({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.version_native_string_encoding_use_utf8,
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8 }) => {
					Support.PopCap.ReflectionObjectNotation.DecodeLenient.process_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 });
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode_lenient'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.popcap.reflection_object_notation.injector;