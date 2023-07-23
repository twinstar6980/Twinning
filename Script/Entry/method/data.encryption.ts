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
				worker(a: Entry.CommonArgument & {
					plain_file: Executor.Argument<string, false>;
					cipher_file: Executor.Argument<string, true>;
					key: Executor.Argument<bigint, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let key: bigint;
					{
						plain_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						cipher_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => (0x00n <= value && value <= 0xFFn ? null : los(`范围溢出`)), initial)),
						);
					}
					KernelX.Tool.Data.Encryption.XOR.encrypt_fs(plain_file, cipher_file, [key]);
					return [`${cipher_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
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
				worker(a: Entry.CommonArgument & {
					plain_file: Executor.Argument<string, false>;
					cipher_file: Executor.Argument<string, true>;
					mode: Executor.Argument<string, false>;
					block_size: Executor.Argument<bigint, false>;
					key: Executor.Argument<string, false>;
					iv: Executor.Argument<string, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let mode: string;
					let block_size: bigint;
					let key: string;
					let iv: string;
					{
						plain_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						cipher_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						mode = Executor.request_argument(
							Executor.query_argument_name(this.id, 'mode'),
							a.mode,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(KernelX.Tool.Data.Encryption.Rijndael.ModeE), null, null, initial)),
						);
						block_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'block_size'),
							a.block_size,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE), null, null, initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, (value) => (KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los(`密钥长度非法`)), initial)),
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
					KernelX.Tool.Data.Encryption.Rijndael.encrypt_fs(plain_file, cipher_file, mode as any, block_size as any, BigInt(key.length) as KernelX.Tool.Data.Encryption.Rijndael.BlockSize, key, iv);
					return [`${cipher_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
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
				worker(a: Entry.CommonArgument & {
					cipher_file: Executor.Argument<string, false>;
					plain_file: Executor.Argument<string, true>;
					mode: Executor.Argument<string, false>;
					block_size: Executor.Argument<bigint, false>;
					key: Executor.Argument<string, false>;
					iv: Executor.Argument<string, false>;
				}) {
					let cipher_file: string;
					let plain_file: string;
					let mode: string;
					let block_size: bigint;
					let key: string;
					let iv: string;
					{
						cipher_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						plain_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							() => (cipher_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						mode = Executor.request_argument(
							Executor.query_argument_name(this.id, 'mode'),
							a.mode,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(KernelX.Tool.Data.Encryption.Rijndael.ModeE), null, null, initial)),
						);
						block_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'block_size'),
							a.block_size,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE), null, null, initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, (value) => (KernelX.Tool.Data.Encryption.Rijndael.BlockSizeE.includes(BigInt(value.length) as any) ? null : los(`密钥长度非法`)), initial)),
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
					KernelX.Tool.Data.Encryption.Rijndael.decrypt_fs(cipher_file, plain_file, mode as any, block_size as any, BigInt(key.length) as KernelX.Tool.Data.Encryption.Rijndael.BlockSize, key, iv);
					return [`${plain_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
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