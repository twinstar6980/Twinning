namespace TwinStar.Script.Entry.method.data.encoding {

	// ------------------------------------------------

	// base64.encode
	// base64.decode

	type Configuration = {
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.encoding.base64.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_file: Executor.RequestArgument<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					CoreX.Tool.Data.Encoding.Base64.encode_fs(raw_file, ripe_file);
					Console.message('s', los(`执行成功`), [`${ripe_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.encoding.base64.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					{
						ripe_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					CoreX.Tool.Data.Encoding.Base64.decode_fs(ripe_file, raw_file);
					Console.message('s', los(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.data.encoding._injector,
});