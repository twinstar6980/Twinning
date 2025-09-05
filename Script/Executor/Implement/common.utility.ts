namespace Twinning.Script.Executor.Implement.common.utility {

	// #region partition function

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
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
				id: 'run_executable_script',
				filter: null,
				argument: [
					typical_argument_string({
						id: 'target',
						option: KernelX.Storage.list_directory(HomePath.of(`~/script/Executable`), null, true, false)
							.filter((value) => (/(\.js)$/i.test(value)))
							.map((value) => (value.replace(/(\.js)$/i, ''))),
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ target }) => {
					KernelX.Miscellaneous.evaluate_fs(HomePath.of(`~/script/Executable/${target}.js`), false);
					return;
				},
				batch_filter: null,
				batch_argument: null,
				batch_worker: null,
			}),
		]);
		return;
	}

	// #endregion

}

Twinning.Script.Executor.Implement.common.utility.injector;