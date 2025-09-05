namespace Twinning.Script.Executor.Implement.data.hash {

	// #region partition function

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'data.hash', [
			typical_method({
				id: 'md5',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ target_file }) => {
					let result = KernelX.Tool.Data.Hash.MD5.hash_fs(target_file);
					Console.success(`${parse_string_from_byte_array(make_integer_to_byte_array(result, 16, 'little'), false)}`, []);
					return;
				},
				batch_filter: null,
				batch_argument: [
					typical_argument_batch({
						id: 'target_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// #endregion

}

Twinning.Script.Executor.Implement.data.hash.injector;