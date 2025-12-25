namespace Twinning.Script.Executor.Implement.Data.Hash {

	// #region interface

	export function activate(
	): void {
		push_typical_method('data.hash', [
			typical_method({
				identifier: 'md5',
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
				worker: ({ target_file }, temporary: {}) => {
					let result = KernelX.Tool.Data.Hash.Md5.hash_fs(target_file);
					Console.success(`${ConvertHelper.parse_string_from_byte_array(ConvertHelper.make_integer_to_byte_array(result, 16, 'little'), false)}`, []);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
