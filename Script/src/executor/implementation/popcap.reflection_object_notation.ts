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
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.json)?$/i, '.rton'),
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
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /()?$/i, '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.json)?$/i, '.rton'),
					}),
				],
				worker: ({definition_file, data_file, version_number, version_native_string_encoding_use_utf8, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Popcap.ReflectionObjectNotation.encode_fs(data_file, definition_file, true, true, {number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8}, store.buffer.view());
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.rton)?$/i, '.json'),
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.decode'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.rton)?$/i, '.json'),
					}),
				],
				worker: ({data_file, definition_file, version_number, version_native_string_encoding_use_utf8}, store: {}) => {
					KernelX.Tool.Popcap.ReflectionObjectNotation.decode_fs(data_file, definition_file, {number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8});
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
						automatic: (argument: {plain_file: StoragePath}) => ConvertHelper.replace_path_name(argument.plain_file, /(\.rton)?$/i, '.cipher.rton'),
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
						automatic: (argument: {plain_file: StoragePath}) => ConvertHelper.replace_path_name(argument.plain_file, /()?$/i, '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.rton)?$/i, '.rton'),
					}),
				],
				worker: ({plain_file, cipher_file, key}, store: {}) => {
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
						automatic: (argument: {cipher_file: StoragePath}) => ConvertHelper.replace_path_name(argument.cipher_file, /(\.rton)?$/i, '.plain.rton'),
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
						automatic: (argument: {cipher_file: StoragePath}) => ConvertHelper.replace_path_name(argument.cipher_file, /()?$/i, '.decrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.rton)?$/i, '.rton'),
					}),
				],
				worker: ({cipher_file, plain_file, key}, store: {}) => {
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
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /(\.json)?$/i, '.rton'),
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
						automatic: (argument: {definition_file: StoragePath}) => ConvertHelper.replace_path_name(argument.definition_file, /()?$/i, '.encode_cipher'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.json)?$/i, '.rton'),
					}),
				],
				worker: ({definition_file, data_file, version_number, version_native_string_encoding_use_utf8, key, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Popcap.ReflectionObjectNotation.encode_cipher_fs(data_file, definition_file, true, true, {number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8}, key, store.buffer.view());
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.rton)?$/i, '.json'),
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.decode_cipher'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.rton)?$/i, '.json'),
					}),
				],
				worker: ({data_file, definition_file, version_number, version_native_string_encoding_use_utf8, key}, store: {}) => {
					KernelX.Tool.Popcap.ReflectionObjectNotation.decode_cipher_fs(data_file, definition_file, {number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8}, key);
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /(\.rton)?$/i, '.json'),
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
						automatic: (argument: {data_file: StoragePath}) => ConvertHelper.replace_path_name(argument.data_file, /()?$/i, '.decode_lenient'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /(\.rton)?$/i, '.json'),
					}),
				],
				worker: ({data_file, definition_file, version_number, version_native_string_encoding_use_utf8}, store: {}) => {
					Support.Popcap.ReflectionObjectNotation.DecodeLenient.process_fs(data_file, definition_file, {number: version_number as any, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8});
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
