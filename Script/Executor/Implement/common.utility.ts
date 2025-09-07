namespace Twinning.Script.Executor.Implement.common.utility {

	// #region partition function

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function inject(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'common.utility', [
			typical_method({
				id: 'set_global_configuration',
				filter: null,
				argument: [
					typical_argument_string({
						id: 'data',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ data }) => {
					Runner.set_configuration(KernelX.JSON.read_s_js(data) as any);
					return;
				},
				batch_filter: null,
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'run_embedded_script',
				filter: null,
				argument: [
					typical_argument_string({
						id: 'target_name',
						option: KernelX.Storage.list_directory(HomePath.of(`~/script/Executable`), null, true, false)
							.filter((value) => (/(\.js)$/i.test(value)))
							.map((value) => (value.replace(/(\.js)$/i, ''))),
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ target_name }) => {
					KernelX.Miscellaneous.evaluate_fs(HomePath.of(`~/script/Executable/${target_name}.js`), false);
					return;
				},
				batch_filter: null,
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'run_custom_script',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'is_module',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ target_file, is_module }) => {
					let result = KernelX.Miscellaneous.evaluate_fs(target_file, is_module);
					Console.success(`> ${result}`, []);
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

Twinning.Script.Executor.Implement.common.utility.inject;