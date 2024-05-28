namespace Twinning.Script.Executor.Implement.popcap.crypt_data {

	// ------------------------------------------------

	// encrypt *
	// decrypt *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'popcap.crypt_data', [
			typical_method({
				id: 'encrypt',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'plain_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'cipher_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/()?$/i, '.cdat')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'limit',
						option: null,
						checker: (argument: {}, value) => (0x00n <= value ? null : los(`范围溢出`)),
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
				worker: ({ plain_file, cipher_file, limit, key }) => {
					KernelX.Tool.PopCap.CryptData.encrypt_fs(plain_file, cipher_file, limit, key, {});
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'plain_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'cipher_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file + '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '.cdat')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'decrypt',
				filter: 'file',
				argument: [
					typical_argument_path({
						id: 'cipher_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'plain_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file.replace(/(\.cdat)?$/i, '')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'limit',
						option: null,
						checker: (argument: {}, value) => (0x00n <= value ? null : los(`范围溢出`)),
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
				worker: ({ cipher_file, plain_file, limit, key }) => {
					KernelX.Tool.PopCap.CryptData.decrypt_fs(cipher_file, plain_file, limit, key, {});
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'cipher_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'plain_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file + '.decrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.cdat)?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

Twinning.Script.Executor.Implement.popcap.crypt_data.injector;