namespace Twinning.Script.Executor.Implementation.Data.Hashing {

	// #region interface

	export function activate(
	): void {
		push_typical_method('data.hashing', [
			typical_method({
				identifier: 'md5.hash',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'input'],
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
				worker: ({target_file}, store: {}) => {
					let result = KernelX.Tool.Data.Hashing.Md5.hash_fs(target_file);
					Console.success(`${ConvertHelper.parse_string_from_byte_array(ConvertHelper.make_integer_to_byte_array(result, 16, 'little'), false)}`, []);
					return;
				},
			}),
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
					typical_argument_string({
						identifier: 'bit_count',
						option: KernelX.Tool.Data.Hashing.Fnv.BitCountE,
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
				worker: ({target_file, mode, bit_count}, store: {}) => {
					let result = KernelX.Tool.Data.Hashing.Fnv.hash_fs(target_file, mode as any, bit_count as any);
					Console.success(`${ConvertHelper.parse_string_from_byte_array(ConvertHelper.make_integer_to_byte_array(result, bit_count === 'b_32' ? 4 : 8, 'little'), false)}`, []);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
