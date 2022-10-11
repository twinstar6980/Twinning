/**
 * + data.encrypt.xor.crypt XOR 加解密
 * + data.encrypt.rijndael.encrypt Rijndael 加密
 * + data.encrypt.rijndael.decrypt Rijndael 解密
 */
namespace TwinKleS.Entry.method.data.encrypt {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.encrypt.xor.crypt',
				description: 'XOR 加解密',
				worker(a: Entry.CFSA & {
					plain_file: Argument.Require<string>;
					cipher_file: Argument.Request<string, true>;
					key: Argument.Request<bigint, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let key: bigint;
					{
						plain_file = Argument.require(
							'明文文件', '',
							a.plain_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						cipher_file = Argument.request(
							'密文文件', '',
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						key = Argument.request(
							'密钥', '',
							a.key,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (0x00n <= value && value <= 0xFFn ? null : `范围溢出`),
						);
					}
					CoreX.Tool.Data.Encrypt.XOR.crypt_fs(plain_file, cipher_file, key);
					Console.notify('s', `执行成功`, [`${cipher_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file: undefined!,
					cipher_file: '?default',
					key: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'plain_file',
			}),
			Executor.method_of({
				id: 'data.encrypt.rijndael.encrypt',
				description: 'Rijndael 加密',
				worker(a: Entry.CFSA & {
					plain_file: Argument.Require<string>;
					cipher_file: Argument.Request<string, true>;
					mode: Argument.Request<string, false>;
					block_size: Argument.Request<bigint, false>;
					key: Argument.Request<string, false>;
					iv: Argument.Request<string, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let mode: CoreX.Tool.Data.Encrypt.Rijndael.Mode;
					let block_size: CoreX.Tool.Data.Encrypt.Rijndael.BlockSize;
					let key: string;
					let iv: string;
					{
						plain_file = Argument.require(
							'明文文件', '',
							a.plain_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						cipher_file = Argument.request(
							'密文文件', '',
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						mode = Argument.request(
							'算法模式', '',
							a.mode,
							(value) => (value),
							null,
							() => (Console.option(CoreX.Tool.Data.Encrypt.Rijndael.ModeE.map((e) => ([e])), null)),
							(value) => (CoreX.Tool.Data.Encrypt.Rijndael.ModeE.includes(value as any) ? null : `选项非法`),
						);
						block_size = Argument.request(
							'块大小', '',
							a.block_size,
							(value) => (value),
							null,
							() => (Console.option(CoreX.Tool.Data.Encrypt.Rijndael.BlockSizeE.map((e) => ([e])), null)),
							(value) => (CoreX.Tool.Data.Encrypt.Rijndael.BlockSizeE.includes(value as any) ? null : `选项非法`),
						);
						key = Argument.request(
							'密钥', '',
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (CoreX.Tool.Data.Encrypt.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : `密钥长度非法`),
						);
						if (mode === 'cbc' || mode === 'cfb') {
							iv = Argument.request(
								'初始向量', '',
								a.iv,
								(value) => (value),
								null,
								() => (Console.string(null)),
								(value) => (value.length === Number(block_size) ? null : `长度不匹配`),
							);
						} else {
							iv = '';
						}
					}
					CoreX.Tool.Data.Encrypt.Rijndael.encrypt_fs(plain_file, cipher_file, mode, block_size, BigInt(key.length) as CoreX.Tool.Data.Encrypt.Rijndael.BlockSize, key, iv);
					Console.notify('s', `执行成功`, [`${cipher_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file: undefined!,
					cipher_file: '?default',
					mode: '?input',
					block_size: '?input',
					key: '?input',
					iv: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'plain_file',
			}),
			Executor.method_of({
				id: 'data.encrypt.rijndael.decrypt',
				description: 'Rijndael 解密',
				worker(a: Entry.CFSA & {
					cipher_file: Argument.Require<string>;
					plain_file: Argument.Request<string, true>;
					mode: Argument.Request<string, false>;
					block_size: Argument.Request<bigint, false>;
					key: Argument.Request<string, false>;
					iv: Argument.Request<string, false>;
				}) {
					let cipher_file: string;
					let plain_file: string;
					let mode: CoreX.Tool.Data.Encrypt.Rijndael.Mode;
					let block_size: CoreX.Tool.Data.Encrypt.Rijndael.BlockSize;
					let key: string;
					let iv: string;
					{
						cipher_file = Argument.require(
							'密文文件', '',
							a.cipher_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						plain_file = Argument.request(
							'明文文件', '',
							a.plain_file,
							(value) => (value),
							() => (cipher_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						mode = Argument.request(
							'算法模式', '',
							a.mode,
							(value) => (value),
							null,
							() => (Console.option(CoreX.Tool.Data.Encrypt.Rijndael.ModeE.map((e) => ([e])), null)),
							(value) => (CoreX.Tool.Data.Encrypt.Rijndael.ModeE.includes(value as any) ? null : `选项非法`),
						);
						block_size = Argument.request(
							'块大小', '',
							a.block_size,
							(value) => (value),
							null,
							() => (Console.option(CoreX.Tool.Data.Encrypt.Rijndael.BlockSizeE.map((e) => ([e])), null)),
							(value) => (CoreX.Tool.Data.Encrypt.Rijndael.BlockSizeE.includes(value as any) ? null : `选项非法`),
						);
						key = Argument.request(
							'密钥', '',
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (CoreX.Tool.Data.Encrypt.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : `密钥长度非法`),
						);
						if (mode === 'cbc' || mode === 'cfb') {
							iv = Argument.request(
								'初始向量', '',
								a.iv,
								(value) => (value),
								null,
								() => (Console.string(null)),
								(value) => (value.length === Number(block_size) ? null : `长度不匹配`),
							);
						} else {
							iv = '';
						}
					}
					CoreX.Tool.Data.Encrypt.Rijndael.decrypt_fs(cipher_file, plain_file, mode, block_size, BigInt(key.length) as CoreX.Tool.Data.Encrypt.Rijndael.BlockSize, key, iv);
					Console.notify('s', `执行成功`, [`${plain_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					cipher_file: undefined!,
					plain_file: '?default',
					mode: '?input',
					block_size: '?input',
					key: '?input',
					iv: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'cipher_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.data.encrypt._injector,
});