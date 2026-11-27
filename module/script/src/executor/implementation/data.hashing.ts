namespace Twinning.Script.Executor.Implementation.Data.Hashing {

	// #region interface

	export function activate(
	): void {
		push_typical_method('data.hashing', [
			typical_method({
				identifier: 'fnv.hash',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'mode',
						option: KernelX.Tool.Data.Hashing.Fnv.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'length',
						option: KernelX.Tool.Data.Hashing.Fnv.LengthE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target_file, mode, length}, store: {}) => {
					let result = KernelX.Tool.Data.Hashing.Fnv.hash_fs(target_file, mode as any, length as any);
					Console.success(`${ConvertHelper.parse_string_from_byte_array(ConvertHelper.make_integer_to_byte_array(result, Number(length / 8n), 'little'), false)}`, []);
					return;
				},
			}),
			typical_method({
				identifier: 'crc.hash',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'mode',
						option: KernelX.Tool.Data.Hashing.Crc.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'length',
						option: KernelX.Tool.Data.Hashing.Crc.LengthE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target_file, mode, length}, store: {}) => {
					let result = KernelX.Tool.Data.Hashing.Crc.hash_fs(target_file, mode as any, length as any);
					Console.success(`${ConvertHelper.parse_string_from_byte_array(ConvertHelper.make_integer_to_byte_array(result, Number(length / 8n), 'little'), false)}`, []);
					return;
				},
			}),
			typical_method({
				identifier: 'adler.hash',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'mode',
						option: KernelX.Tool.Data.Hashing.Adler.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'length',
						option: KernelX.Tool.Data.Hashing.Adler.LengthE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target_file, mode, length}, store: {}) => {
					let result = KernelX.Tool.Data.Hashing.Adler.hash_fs(target_file, mode as any, length as any);
					Console.success(`${ConvertHelper.parse_string_from_byte_array(ConvertHelper.make_integer_to_byte_array(result, Number(length / 8n), 'little'), false)}`, []);
					return;
				},
			}),
			typical_method({
				identifier: 'md.hash',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'mode',
						option: KernelX.Tool.Data.Hashing.Md.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'length',
						option: KernelX.Tool.Data.Hashing.Md.LengthE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target_file, mode, length}, store: {}) => {
					let result = KernelX.Tool.Data.Hashing.Md.hash_fs(target_file, mode as any, length as any);
					Console.success(`${ConvertHelper.parse_string_from_byte_array(ConvertHelper.make_integer_to_byte_array(result, Number(length / 8n), 'little'), false)}`, []);
					return;
				},
			}),
			typical_method({
				identifier: 'sha.hash',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'mode',
						option: KernelX.Tool.Data.Hashing.Sha.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'length',
						option: KernelX.Tool.Data.Hashing.Sha.LengthE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target_file, mode, length}, store: {}) => {
					let result = KernelX.Tool.Data.Hashing.Sha.hash_fs(target_file, mode as any, length as any);
					Console.success(`${ConvertHelper.parse_string_from_byte_array(ConvertHelper.make_integer_to_byte_array(result, Number(length / 8n), 'little'), false)}`, []);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
