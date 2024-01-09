namespace TwinStar.Script.Executor.Implement.kairosoft.game {

	// ------------------------------------------------

	// encrypt_record
	// modify_program

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'kairosoft.game', [
			typical_method({
				id: 'encrypt_record',
				filter: 'directory',
				argument: [
					typical_argument_path({
						id: 'target_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ target_directory, key }) => {
					Support.Kairosoft.Game.EncryptRecord.process_fs(target_directory, BigInt(key));
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'modify_program',
				filter: 'directory',
				argument: [
					typical_argument_path({
						id: 'target_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'disable_record_encryption',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'enable_debug_mode',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ target_directory, disable_record_encryption, enable_debug_mode }) => {
					Support.Kairosoft.Game.ModifyProgram.process_fs(target_directory, disable_record_encryption, enable_debug_mode);
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

TwinStar.Script.Executor.Implement.kairosoft.game.injector;