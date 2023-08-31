namespace TwinStar.Script.Executor.Implement.data.encryption {

	// ------------------------------------------------

	// xor.encrypt *
	// rijndael.encrypt *
	// rijndael.decrypt *

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('data.encryption', [
			typical_method({
				id: 'xor.encrypt',
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
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/()?$/i, '.bin')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'key',
						option: null,
						checker: (argument: {}, value) => (0x00n <= value && value <= 0xFFn ? null : los(`范围溢出`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ plain_file, cipher_file, key }) => {
					KernelX.Tool.Data.Encryption.XOR.encrypt_fs(plain_file, cipher_file, [key]);
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
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'rijndael.encrypt',
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
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/()?$/i, '.bin')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'mode',
						option: KernelX.Tool.Data.Encryption.Rijndael.ModeE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_integer({
						id: 'block_size',
						option: KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: (argument: {}, value) => (KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los(`密钥长度非法`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'iv',
						option: null,
						checker: (argument: { block_size: bigint; }, value) => (value.length === Number(argument.block_size) ? null : los(`长度不匹配`)),
						automatic: null,
						condition: (argument: { mode: string; }) => (argument.mode === 'cbc' || argument.mode === 'cfb' ? null : ''),
						default: '?input',
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
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'rijndael.decrypt',
				filter: ['file', /()$/i],
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
						automatic: (argument: { cipher_file: string; }) => (argument.cipher_file.replace(/()?$/i, '.bin')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'mode',
						option: KernelX.Tool.Data.Encryption.Rijndael.ModeE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_integer({
						id: 'block_size',
						option: KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'key',
						option: null,
						checker: (argument: {}, value) => (KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los(`密钥长度非法`)),
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'iv',
						option: null,
						checker: (argument: { block_size: bigint; }, value) => (value.length === Number(argument.block_size) ? null : los(`长度不匹配`)),
						automatic: null,
						condition: (argument: { mode: string; }) => (argument.mode === 'cbc' || argument.mode === 'cfb' ? null : ''),
						default: '?input',
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

TwinStar.Script.Executor.Implement.data.encryption.injector;