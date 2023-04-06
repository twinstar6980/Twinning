namespace TwinStar.Script.Entry.method.data.encryption {

	// ------------------------------------------------

	// xor.encrypt
	// rijndael.encrypt
	// rijndael.decrypt

	type Configuration = {
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.encryption.xor.encrypt',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					plain_file: Executor.RequireArgument<string>;
					cipher_file: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<bigint, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let key: bigint;
					{
						plain_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						cipher_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => (0x00n <= value && value <= 0xFFn ? null : los(`范围溢出`)), initial)),
						);
					}
					CoreX.Tool.Data.Encryption.XOR.encrypt_fs(plain_file, cipher_file, [key]);
					Console.message('s', los(`执行成功`), [`${cipher_file}`]);
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
				id: 'data.encryption.rijndael.encrypt',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					plain_file: Executor.RequireArgument<string>;
					cipher_file: Executor.RequestArgument<string, true>;
					mode: Executor.RequestArgument<string, false>;
					block_size: Executor.RequestArgument<bigint, false>;
					key: Executor.RequestArgument<string, false>;
					iv: Executor.RequestArgument<string, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let mode: string;
					let block_size: bigint;
					let key: string;
					let iv: string;
					{
						plain_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						cipher_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						mode = Executor.request_argument(
							Executor.query_argument_name(this.id, 'mode'),
							a.mode,
							(value) => (value),
							null,
							(initial) => (Console.option(CoreX.Tool.Data.Encryption.Rijndael.ModeE.map((e) => ([e])), null, null, initial)),
						);
						block_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'block_size'),
							a.block_size,
							(value) => (value),
							null,
							(initial) => (Console.option(CoreX.Tool.Data.Encryption.Rijndael.BlockSizeE.map((e) => ([e])), null, null, initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, (value) => (CoreX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los(`密钥长度非法`)), initial)),
						);
						if (mode === 'cbc' || mode === 'cfb') {
							iv = Executor.request_argument(
								Executor.query_argument_name(this.id, 'iv'),
								a.iv,
								(value) => (value),
								null,
								(initial) => (Console.string(null, (value) => (value.length === Number(block_size) ? null : los(`长度不匹配`)), initial)),
								
							);
						} else {
							iv = '';
						}
					}
					CoreX.Tool.Data.Encryption.Rijndael.encrypt_fs(plain_file, cipher_file, mode as any, block_size as any, BigInt(key.length) as CoreX.Tool.Data.Encryption.Rijndael.BlockSize, key, iv);
					Console.message('s', los(`执行成功`), [`${cipher_file}`]);
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
				id: 'data.encryption.rijndael.decrypt',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					cipher_file: Executor.RequireArgument<string>;
					plain_file: Executor.RequestArgument<string, true>;
					mode: Executor.RequestArgument<string, false>;
					block_size: Executor.RequestArgument<bigint, false>;
					key: Executor.RequestArgument<string, false>;
					iv: Executor.RequestArgument<string, false>;
				}) {
					let cipher_file: string;
					let plain_file: string;
					let mode: string;
					let block_size: bigint;
					let key: string;
					let iv: string;
					{
						cipher_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						plain_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							() => (cipher_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						mode = Executor.request_argument(
							Executor.query_argument_name(this.id, 'mode'),
							a.mode,
							(value) => (value),
							null,
							(initial) => (Console.option(CoreX.Tool.Data.Encryption.Rijndael.ModeE.map((e) => ([e])), null, null, initial)),
						);
						block_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'block_size'),
							a.block_size,
							(value) => (value),
							null,
							(initial) => (Console.option(CoreX.Tool.Data.Encryption.Rijndael.BlockSizeE.map((e) => ([e])), null, null, initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, (value) => (CoreX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los(`密钥长度非法`)), initial)),
						);
						if (mode === 'cbc' || mode === 'cfb') {
							iv = Executor.request_argument(
								Executor.query_argument_name(this.id, 'iv'),
								a.iv,
								(value) => (value),
								null,
								(initial) => (Console.string(null, (value) => (value.length === Number(block_size) ? null : los(`长度不匹配`)), initial)),
								
							);
						} else {
							iv = '';
						}
					}
					CoreX.Tool.Data.Encryption.Rijndael.decrypt_fs(cipher_file, plain_file, mode as any, block_size as any, BigInt(key.length) as CoreX.Tool.Data.Encryption.Rijndael.BlockSize, key, iv);
					Console.message('s', los(`执行成功`), [`${plain_file}`]);
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
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.data.encryption._injector,
});