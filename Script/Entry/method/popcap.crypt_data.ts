namespace TwinStar.Script.Entry.method.popcap.crypt_data {

	// ------------------------------------------------

	// encrypt
	// decrypt

	type Configuration = {
		limit: Executor.Argument<bigint, false>;
		key: Executor.Argument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.crypt_data.encrypt',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					plain_file: Executor.Argument<string, false>;
					cipher_file: Executor.Argument<string, true>;
					limit: Executor.Argument<bigint, false>;
					key: Executor.Argument<string, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let limit: bigint;
					let key: string;
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
							() => (plain_file.replace(/()?$/i, '.cdat')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						limit = Executor.request_argument(
							Executor.query_argument_name(this.id, 'limit'),
							a.limit,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => (0x00n <= value ? null : los(`范围溢出`)), initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.CryptData.encrypt_fs(plain_file, cipher_file, limit, key, {});
					return [`${cipher_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					plain_file: undefined!,
					cipher_file: '?default',
					limit: configuration.limit,
					key: configuration.key,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'plain_file',
			}),
			Executor.method_of({
				id: 'popcap.crypt_data.decrypt',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					cipher_file: Executor.Argument<string, false>;
					plain_file: Executor.Argument<string, true>;
					limit: Executor.Argument<bigint, false>;
					key: Executor.Argument<string, false>;
				}) {
					let cipher_file: string;
					let plain_file: string;
					let limit: bigint;
					let key: string;
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
							() => (cipher_file.replace(/((\.cdat))?$/i, '')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						limit = Executor.request_argument(
							Executor.query_argument_name(this.id, 'limit'),
							a.limit,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => (0x00n <= value ? null : los(`范围溢出`)), initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.CryptData.decrypt_fs(cipher_file, plain_file, limit, key, {});
					return [`${plain_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					cipher_file: undefined!,
					plain_file: '?default',
					limit: configuration.limit,
					key: configuration.key,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.cdat)$/i]]),
				input_forwarder: 'cipher_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.crypt_data.encrypt.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					plain_file_directory: Executor.Argument<string, false>;
					cipher_file_directory: Executor.Argument<string, true>;
					limit: Executor.Argument<bigint, false>;
					key: Executor.Argument<string, false>;
				}) {
					let plain_file_directory: string;
					let cipher_file_directory: string;
					let limit: bigint;
					let key: string;
					{
						plain_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'plain_file_directory'),
							a.plain_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						cipher_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'cipher_file_directory'),
							a.cipher_file_directory,
							(value) => (value),
							() => (plain_file_directory.replace(/$/i, '.encrypt')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						limit = Executor.request_argument(
							Executor.query_argument_name(this.id, 'limit'),
							a.limit,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => (0x00n <= value ? null : los(`范围溢出`)), initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, null, initial)),
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
					return [`${cipher_file_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					plain_file_directory: undefined!,
					cipher_file_directory: '?default',
					limit: configuration.limit,
					key: configuration.key,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'plain_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.crypt_data.decrypt.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					cipher_file_directory: Executor.Argument<string, false>;
					plain_file_directory: Executor.Argument<string, true>;
					limit: Executor.Argument<bigint, false>;
					key: Executor.Argument<string, false>;
				}) {
					let cipher_file_directory: string;
					let plain_file_directory: string;
					let limit: bigint;
					let key: string;
					{
						cipher_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'cipher_file_directory'),
							a.cipher_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						plain_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'plain_file_directory'),
							a.plain_file_directory,
							(value) => (value),
							() => (cipher_file_directory.replace(/$/i, '.decrypt')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						limit = Executor.request_argument(
							Executor.query_argument_name(this.id, 'limit'),
							a.limit,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => (0x00n <= value ? null : los(`范围溢出`)), initial)),
						);
						key = Executor.request_argument(
							Executor.query_argument_name(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							(initial) => (Console.string(null, null, initial)),
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
					return [`${plain_file_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					cipher_file_directory: undefined!,
					plain_file_directory: '?default',
					limit: configuration.limit,
					key: configuration.key,
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