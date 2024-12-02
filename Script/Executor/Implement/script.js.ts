namespace Twinning.Script.Executor.Implement.script.js {

	// ------------------------------------------------

	// execute *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'script.js', [
			typical_method({
				id: 'execute',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'script_file',
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
				worker: ({ script_file, is_module }) => {
					let result = KernelX.Miscellaneous.evaluate_fs(script_file, is_module);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'script_file',
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

Twinning.Script.Executor.Implement.script.js.injector;