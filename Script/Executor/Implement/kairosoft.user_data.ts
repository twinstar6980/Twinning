namespace TwinStar.Script.Executor.Implement.kairosoft.user_data {

	// ------------------------------------------------

	// recrypt *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'kairosoft.user_data', [
			typical_method({
				id: 'recrypt',
				filter: 'directory',
				argument: [
					typical_argument_path({
						id: 'old_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'new_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { old_directory: string; }) => (argument.old_directory.replace(/()?$/i, '.recrypt')),
						condition: null,
					}),
					typical_argument_string({
						id: 'old_key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'new_key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ old_directory, new_directory, old_key, new_key }) => {
					Support.Kairosoft.UserData.Recrypt.process_fs(old_directory, new_directory, BigInt(old_key), BigInt(new_key));
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.kairosoft.user_data.injector;