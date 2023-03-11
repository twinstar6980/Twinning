namespace TwinStar.Script.Entry.method.popcap.zlib {

	// ------------------------------------------------

	// compress *
	// uncompress *

	type Config = {
		version_variant_64: Executor.RequestArgument<boolean, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.zlib.compress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_file: Executor.RequestArgument<string, true>;
					version_variant_64: Executor.RequestArgument<boolean, false>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					let version_variant_64: boolean;
					{
						raw_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_variant_64 = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.ZLib.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', { variant_64: version_variant_64 });
					Console.notify('s', los(`执行成功`), [`${ripe_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
					version_variant_64: config.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'popcap.zlib.uncompress',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
					version_variant_64: Executor.RequestArgument<boolean, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let version_variant_64: boolean;
					{
						ripe_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						raw_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_variant_64 = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.ZLib.uncompress_fs(ripe_file, raw_file, 15n, { variant_64: version_variant_64 });
					Console.notify('s', los(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
					version_variant_64: config.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.zlib.compress.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file_directory: Executor.RequireArgument<string>;
					ripe_file_directory: Executor.RequestArgument<string, true>;
					version_variant_64: Executor.RequestArgument<boolean, false>;
				}) {
					let raw_file_directory: string;
					let ripe_file_directory: string;
					let version_variant_64: boolean;
					{
						raw_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						ripe_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'ripe_file_directory'),
							a.ripe_file_directory,
							(value) => (value),
							() => (raw_file_directory.replace(/$/i, '.popcap_zlib_compress')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_variant_64 = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						raw_file_directory,
						['file', null],
						(item) => {
							let raw_file = `${raw_file_directory}/${item}`;
							let ripe_file = `${ripe_file_directory}/${item}`;
							CoreX.Tool.PopCap.ZLib.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', { variant_64: version_variant_64 });
						},
					);
					Console.notify('s', los(`执行成功`), [`${ripe_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file_directory: undefined!,
					ripe_file_directory: '?default',
					version_variant_64: config.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'raw_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.zlib.uncompress.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_file_directory: Executor.RequireArgument<string>;
					raw_file_directory: Executor.RequestArgument<string, true>;
					version_variant_64: Executor.RequestArgument<boolean, false>;
				}) {
					let ripe_file_directory: string;
					let raw_file_directory: string;
					let version_variant_64: boolean;
					{
						ripe_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'ripe_file_directory'),
							a.ripe_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_file_directory.replace(/$/i, '.popcap_zlib_uncompress')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_variant_64 = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						ripe_file_directory,
						['file', null],
						(item) => {
							let ripe_file = `${ripe_file_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item}`;
							CoreX.Tool.PopCap.ZLib.uncompress_fs(ripe_file, raw_file, 15n, { variant_64: version_variant_64 });
						},
					);
					Console.notify('s', los(`执行成功`), [`${raw_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file_directory: undefined!,
					raw_file_directory: '?default',
					version_variant_64: config.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'ripe_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.zlib._injector,
});