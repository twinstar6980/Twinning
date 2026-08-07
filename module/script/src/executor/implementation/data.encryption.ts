namespace Twinning.Script.Executor.Implementation.Data.Encryption {

	// #region interface

	export function activate(
	): void {
		push_typical_method('data.encryption', [
			typical_method({
				identifier: 'exor.encrypt',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.bin'),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
						checker: (argument: {}, value) => ((/^(( )*[0-9a-fA-F]{2,2}( )*)+$/.test(value)) ? null : los('executor.implementation:*.illegal_key')),
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({raw_file, ripe_file, key}, store: {}) => {
					KernelX.Tool.Data.Encryption.Exor.encrypt_fs(raw_file, ripe_file, ConvertHelper.make_string_to_byte_array(key));
					return;
				},
			}),
			typical_method({
				identifier: 'rijndael.encrypt',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.bin'),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'mode',
						option: KernelX.Tool.Data.Encryption.Rijndael.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'block_size',
						option: KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
						checker: (argument: {}, value) => (KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los('executor.implementation:*.illegal_key_length')),
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'initialization_vector',
						option: null,
						checker: (argument: {block_size: bigint}, value) => (value.length === Number(argument.block_size) ? null : los('executor.implementation:*.length_not_match')),
						automatic: null,
						condition: (argument: {mode: string}) => (['cbc', 'cfb'].includes(argument.mode) ? null : ''),
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({raw_file, ripe_file, mode, block_size, key, initialization_vector}, store: {}) => {
					let key_data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(key));
					let initialization_vector_data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(initialization_vector));
					KernelX.Tool.Data.Encryption.Rijndael.encrypt_fs(raw_file, ripe_file, mode as any, block_size as any, key_data.view(), initialization_vector_data.view());
					return;
				},
			}),
			typical_method({
				identifier: 'rijndael.decrypt',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.bin'),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'mode',
						option: KernelX.Tool.Data.Encryption.Rijndael.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'block_size',
						option: KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'key',
						option: null,
						checker: (argument: {}, value) => (KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los('executor.implementation:*.illegal_key_length')),
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'initialization_vector',
						option: null,
						checker: (argument: {block_size: bigint}, value) => (value.length === Number(argument.block_size) ? null : los('executor.implementation:*.length_not_match')),
						automatic: null,
						condition: (argument: {mode: string}) => (['cbc', 'cfb'].includes(argument.mode) ? null : ''),
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.decrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({ripe_file, raw_file, mode, block_size, key, initialization_vector}, store: {}) => {
					let key_data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(key));
					let initialization_vector_data = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(initialization_vector));
					KernelX.Tool.Data.Encryption.Rijndael.decrypt_fs(raw_file, ripe_file, mode as any, block_size as any, key_data.view(), initialization_vector_data.view());
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
