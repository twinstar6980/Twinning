namespace Twinning.Script.Executor.Implement.PopCap.ReflectionObjectNotation {

	// #region interface

	export function activate(
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
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.json)?$/i, '.rton')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.json)?$/i, '.rton')),
					}),
				],
				worker: ({ definition_file, data_file, version_number, version_native_string_encoding_use_utf8, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
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
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8 }, temporary: {}) => {
					KernelX.Tool.PopCap.ReflectionObjectNotation.decode_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 });
					return;
				},
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
					}),
					typical_argument_path({
						id: 'cipher_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/(\.rton)?$/i, '.cipher.rton')),
						condition: null,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'plain_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'cipher_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file + '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.rton')),
					}),
				],
				worker: ({ plain_file, cipher_file, key }, temporary: {}) => {
					KernelX.Tool.Miscellaneous.PvZ2CNCryptData.encrypt_fs(plain_file, cipher_file, key);
					return;
				},
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
					}),
					typical_argument_path({
						id: 'plain_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file.replace(/(\.rton)?$/i, '.plain.rton')),
						condition: null,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'cipher_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'plain_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file + '.decrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.rton')),
					}),
				],
				worker: ({ cipher_file, plain_file, key }, temporary: {}) => {
					KernelX.Tool.Miscellaneous.PvZ2CNCryptData.decrypt_fs(cipher_file, plain_file, key);
					return;
				},
			}),
			typical_method({
				id: 'encode_cipher',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.json)?$/i, '.rton')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file + '.encode_cipher'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.json)?$/i, '.rton')),
					}),
				],
				worker: ({ definition_file, data_file, version_number, version_native_string_encoding_use_utf8, key, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.PopCap.ReflectionObjectNotation.encode_cipher_fs(data_file, definition_file, true, true, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, key, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'decode_cipher',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode_cipher'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8, key }, temporary: {}) => {
					KernelX.Tool.PopCap.ReflectionObjectNotation.decode_cipher_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, key);
					return;
				},
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
					}),
					typical_argument_path({
						id: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode_lenient'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8 }, temporary: {}) => {
					Support.PopCap.ReflectionObjectNotation.DecodeLenient.process_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 });
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
