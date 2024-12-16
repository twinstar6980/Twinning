namespace Twinning.Script.Executor.Implement.data.encryption {

	// ------------------------------------------------

	// exor.encrypt *
	// rijndael.encrypt *
	// rijndael.decrypt *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'data.encryption', [
			typical_method({
				id: 'exor.encrypt',
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
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: (argument: {}, value) => ((/^(( )*[0-9a-fA-F]{2,2}( )*)+$/.test(value)) ? null : los(`密钥非法`)),
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ plain_file, cipher_file, key }) => {
					KernelX.Tool.Data.Encryption.EXOR.encrypt_fs(plain_file, cipher_file, string_to_byte_array(key));
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
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'rijndael.encrypt',
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
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
					typical_argument_string({
						id: 'mode',
						option: KernelX.Tool.Data.Encryption.Rijndael.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						id: 'block_size',
						option: KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: (argument: {}, value) => (KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los(`密钥长度非法`)),
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'iv',
						option: null,
						checker: (argument: { block_size: bigint; }, value) => (value.length === Number(argument.block_size) ? null : los(`长度不匹配`)),
						automatic: null,
						condition: (argument: { mode: string; }) => (['cbc', 'cfb'].includes(argument.mode) ? null : ''),
					}),
				],
				worker: ({ plain_file, cipher_file, mode, block_size, key, iv }) => {
					KernelX.Tool.Data.Encryption.Rijndael.encrypt_fs(plain_file, cipher_file, mode as any, block_size as any, BigInt(key.length) as any, key, iv);
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
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'rijndael.decrypt',
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
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
					typical_argument_string({
						id: 'mode',
						option: KernelX.Tool.Data.Encryption.Rijndael.ModeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						id: 'block_size',
						option: KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: (argument: {}, value) => (KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los(`密钥长度非法`)),
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'iv',
						option: null,
						checker: (argument: { block_size: bigint; }, value) => (value.length === Number(argument.block_size) ? null : los(`长度不匹配`)),
						automatic: null,
						condition: (argument: { mode: string; }) => (['cbc', 'cfb'].includes(argument.mode) ? null : ''),
					}),
				],
				worker: ({ cipher_file, plain_file, mode, block_size, key, iv }) => {
					KernelX.Tool.Data.Encryption.Rijndael.decrypt_fs(cipher_file, plain_file, mode as any, block_size as any, BigInt(key.length) as any, key, iv);
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
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

Twinning.Script.Executor.Implement.data.encryption.injector;