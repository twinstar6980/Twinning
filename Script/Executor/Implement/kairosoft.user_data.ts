namespace TwinStar.Script.Executor.Implement.kairosoft.user_data {

	// ------------------------------------------------

	// recrypt *

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('kairosoft.user_data', [
			typical_method({
				id: 'recrypt',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						id: 'old_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'new_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { old_directory: string; }) => (argument.old_directory.replace(/()?$/i, '.recrypt')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'old_key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'new_key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
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