namespace Twinning.Script.Executor.Implementation.Common.Utility {

	// #region interface

	export function activate(
	): void {
		push_typical_method('common.utility', [
			typical_method({
				identifier: 'update_global_setting',
				filter: null,
				argument: [
					typical_argument_string({
						identifier: 'data',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ data }, temporary: {}) => {
					update_setting(KernelX.Json.read_s_js(data) as Partial<Setting>, MainScript.g_setting);
					return;
				},
			}),
			typical_method({
				identifier: 'run_embedded_script',
				filter: null,
				argument: [
					typical_argument_string({
						identifier: 'target_name',
						option: Object.keys(Executable),
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ target_name }, temporary: {}) => {
					Executable[target_name as keyof typeof Executable].run();
					return;
				},
			}),
			typical_method({
				identifier: 'run_custom_script',
				filter: ['file', /(\.js)$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'is_module',
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
				worker: ({ target_file, is_module }, temporary: {}) => {
					let result = KernelX.Miscellaneous.evaluate_fs(target_file, is_module);
					Console.success(`> ${result}`, []);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
