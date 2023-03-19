namespace TwinStar.Script.Entry.method.popcap.crypt_data {

	// ------------------------------------------------

	// encrypt
	// decrypt

	type Config = {
		limit: Executor.RequestArgument<bigint, false>;
		key: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.crypt_data.encrypt',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					plain_file: Executor.RequireArgument<string>;
					cipher_file: Executor.RequestArgument<string, true>;
					limit: Executor.RequestArgument<bigint, false>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let limit: bigint;
					let key: string;
					{
						plain_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						cipher_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/()?$/i, '.cdat')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						limit = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'limit'),
							a.limit,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (0x00n <= value ? null : los(`范围溢出`)),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.CryptData.encrypt_fs(plain_file, cipher_file, limit, key, {});
					Console.notify('s', los(`执行成功`), [`${cipher_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file: undefined!,
					cipher_file: '?default',
					limit: config.limit,
					key: config.key,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'plain_file',
			}),
			Executor.method_of({
				id: 'popcap.crypt_data.decrypt',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					cipher_file: Executor.RequireArgument<string>;
					plain_file: Executor.RequestArgument<string, true>;
					limit: Executor.RequestArgument<bigint, false>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let cipher_file: string;
					let plain_file: string;
					let limit: bigint;
					let key: string;
					{
						cipher_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						plain_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							() => (cipher_file.replace(/((\.cdat))?$/i, '')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						limit = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'limit'),
							a.limit,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (0x00n <= value ? null : los(`范围溢出`)),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.CryptData.decrypt_fs(cipher_file, plain_file, limit, key, {});
					Console.notify('s', los(`执行成功`), [`${plain_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					cipher_file: undefined!,
					plain_file: '?default',
					limit: config.limit,
					key: config.key,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.cdat)$/i]]),
				input_forwarder: 'cipher_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.crypt_data.encrypt.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					plain_file_directory: Executor.RequireArgument<string>;
					cipher_file_directory: Executor.RequestArgument<string, true>;
					limit: Executor.RequestArgument<bigint, false>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let plain_file_directory: string;
					let cipher_file_directory: string;
					let limit: bigint;
					let key: string;
					{
						plain_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'plain_file_directory'),
							a.plain_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						cipher_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'cipher_file_directory'),
							a.cipher_file_directory,
							(value) => (value),
							() => (plain_file_directory.replace(/$/i, '.encrypt')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						limit = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'limit'),
							a.limit,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (0x00n <= value ? null : los(`范围溢出`)),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						plain_file_directory,
						['file', null],
						(item) => {
							let plain_file = `${plain_file_directory}/${item}`;
							let cipher_file = `${cipher_file_directory}/${item}.cdat`;
							CoreX.Tool.PopCap.CryptData.encrypt_fs(plain_file, cipher_file, limit, key, {});
						},
					);
					Console.notify('s', los(`执行成功`), [`${cipher_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file_directory: undefined!,
					cipher_file_directory: '?default',
					limit: config.limit,
					key: config.key,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'plain_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.crypt_data.decrypt.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					cipher_file_directory: Executor.RequireArgument<string>;
					plain_file_directory: Executor.RequestArgument<string, true>;
					limit: Executor.RequestArgument<bigint, false>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let cipher_file_directory: string;
					let plain_file_directory: string;
					let limit: bigint;
					let key: string;
					{
						cipher_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'cipher_file_directory'),
							a.cipher_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						plain_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'plain_file_directory'),
							a.plain_file_directory,
							(value) => (value),
							() => (cipher_file_directory.replace(/$/i, '.decrypt')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						limit = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'limit'),
							a.limit,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (0x00n <= value ? null : los(`范围溢出`)),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						cipher_file_directory,
						['file', /.+(\.cdat)$/i],
						(item) => {
							let cipher_file = `${cipher_file_directory}/${item}`;
							let plain_file = `${plain_file_directory}/${item.slice(0, -5)}`;
							CoreX.Tool.PopCap.CryptData.decrypt_fs(cipher_file, plain_file, limit, key, {});
						},
					);
					Console.notify('s', los(`执行成功`), [`${plain_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					cipher_file_directory: undefined!,
					plain_file_directory: '?default',
					limit: config.limit,
					key: config.key,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'cipher_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.crypt_data._injector,
});