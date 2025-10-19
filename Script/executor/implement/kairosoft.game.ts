namespace Twinning.Script.Executor.Implement.Kairosoft.Game {

	// #region interface

	export function activate(
	): void {
		push_typical_method('kairosoft.game', [
			typical_method({
				id: 'encrypt_record',
				filter: ['directory', /()$/i],
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
						checker: (argument: {}, value) => ((/(^(( )*[0-9a-fA-F]{2,2}( )*)+$)|(^d(32|64):[0-9]+$)/.test(value)) ? null : los('executor.implement:*.illegal_key')),
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ target_directory, key }, temporary: {}) => {
					let key_value = [] as Array<bigint>;
					if (key.startsWith('d32:')) {
						key_value = ConvertHelper.make_integer_to_byte_array(BigInt(key.substring('d32:'.length)), 4);
					}
					else if (key.startsWith('d64:')) {
						key_value = ConvertHelper.make_integer_to_byte_array(BigInt(key.substring('d64:'.length)), 8);
					}
					else {
						key_value = ConvertHelper.make_string_to_byte_array(key);
					}
					Support.Kairosoft.Game.EncryptRecord.process_fs(target_directory, key_value);
					return;
				},
			}),
			typical_method({
				id: 'modify_program',
				filter: ['directory', /()$/i],
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
				batch: null,
				worker: ({ target_directory, disable_record_encryption, enable_debug_mode }, temporary: {}) => {
					Support.Kairosoft.Game.ModifyProgram.process_fs(target_directory, disable_record_encryption, enable_debug_mode);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}