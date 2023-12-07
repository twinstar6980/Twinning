namespace TwinStar.Script.Executor.Implement.data.hash {

	// ------------------------------------------------

	// md5 *

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
					Console.success(`${make_prefix_padded_string(result.toString(16), '0', 16)}`, []);
					return;
				},
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

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.data.hash.injector;