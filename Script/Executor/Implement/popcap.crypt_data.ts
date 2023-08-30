namespace TwinStar.Script.Executor.Implement.popcap.crypt_data {

	// ------------------------------------------------

	// encrypt *
	// decrypt *

	export type Configuration = {
		limit: TypicalArgumentExpression<bigint>;
		key: TypicalArgumentExpression<string>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('popcap.crypt_data', [
			typical_method({
				id: 'encrypt',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'plain_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'cipher_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/()?$/i, '.cdat')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'limit',
						option: null,
						checker: (argument: {}, value) => (0x00n <= value ? null : los(`范围溢出`)),
						automatic: null,
						condition: null,
						default: configuration.limit,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.key,
					}),
				],
				worker: (argument) => {
					let { plain_file, cipher_file, limit, key } = argument;
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
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'cipher_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file + '.encrypt'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '.cdat')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'decrypt',
				filter: ['file', /(\.cdat)$/i],
				argument: [
					typical_argument_path({
						id: 'cipher_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'plain_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file.replace(/(\.cdat)?$/i, '')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'limit',
						option: null,
						checker: (argument: {}, value) => (0x00n <= value ? null : los(`范围溢出`)),
						automatic: null,
						condition: null,
						default: configuration.limit,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.key,
					}),
				],
				worker: (argument) => {
					let { cipher_file, plain_file, limit, key } = argument;
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
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'plain_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file + '.decrypt'),
						condition: null,
						default: '?automatic',
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

TwinStar.Script.Executor.Implement.popcap.crypt_data.injector;