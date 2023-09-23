namespace TwinStar.Script.Executor.Implement.js {

	// ------------------------------------------------

	// execute *

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('js', [
			typical_method({
				id: 'execute',
				filter: ['file', /(\.js)$/i],
				argument: [
					typical_argument_path({
						id: 'script_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_confirmation({
						id: 'is_module',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
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
						default: '?input',
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

TwinStar.Script.Executor.Implement.js.injector;