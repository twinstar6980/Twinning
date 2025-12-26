namespace Twinning.Script.Executor.Implementation.Popcap.ReflectionObjectNotation {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.reflection_object_notation', [
			typical_method({
				identifier: 'encode',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.json)?$/i, '.rton')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'data_file',
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
					KernelX.Tool.Popcap.ReflectionObjectNotation.encode_fs(data_file, definition_file, true, true, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'decode',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8 }, temporary: {}) => {
					KernelX.Tool.Popcap.ReflectionObjectNotation.decode_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 });
					return;
				},
			}),
			typical_method({
				identifier: 'encrypt',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						identifier: 'plain_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'cipher_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/(\.rton)?$/i, '.cipher.rton')),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'plain_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'cipher_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file + '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.rton')),
					}),
				],
				worker: ({ plain_file, cipher_file, key }, temporary: {}) => {
					KernelX.Tool.Miscellaneous.Pvz2cnCryptData.encrypt_fs(plain_file, cipher_file, key);
					return;
				},
			}),
			typical_method({
				identifier: 'decrypt',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						identifier: 'cipher_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'plain_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file.replace(/(\.rton)?$/i, '.plain.rton')),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'cipher_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'plain_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file + '.decrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.rton')),
					}),
				],
				worker: ({ cipher_file, plain_file, key }, temporary: {}) => {
					KernelX.Tool.Miscellaneous.Pvz2cnCryptData.decrypt_fs(cipher_file, plain_file, key);
					return;
				},
			}),
			typical_method({
				identifier: 'encode_cipher',
				filter: ['file', /(\.json)$/i],
				argument: [
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { definition_file: string; }) => (argument.definition_file.replace(/(\.json)?$/i, '.rton')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'data_file',
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
					KernelX.Tool.Popcap.ReflectionObjectNotation.encode_cipher_fs(data_file, definition_file, true, true, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, key, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'decode_cipher',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode_cipher'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8, key }, temporary: {}) => {
					KernelX.Tool.Popcap.ReflectionObjectNotation.decode_cipher_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 }, key);
					return;
				},
			}),
			typical_method({
				identifier: 'decode_lenient',
				filter: ['file', /(\.rton)$/i],
				argument: [
					typical_argument_path({
						identifier: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'definition_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.rton)?$/i, '.json')),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'version_number',
						option: KernelX.Tool.Popcap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'definition_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.decode_lenient'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.rton)?$/i, '.json')),
					}),
				],
				worker: ({ data_file, definition_file, version_number, version_native_string_encoding_use_utf8 }, temporary: {}) => {
					Support.Popcap.ReflectionObjectNotation.DecodeLenient.process_fs(data_file, definition_file, { number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8 });
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
