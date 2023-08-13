namespace TwinStar.Script.Entry.method.popcap.zlib {

	// ------------------------------------------------

	// compress *
	// uncompress *

	type Configuration = {
		version_variant_64: Executor.Argument<boolean, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.zlib.compress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					raw_file: Executor.Argument<string, false>;
					ripe_file: Executor.Argument<string, true>;
					version_variant_64: Executor.Argument<boolean, false>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					let version_variant_64: boolean;
					{
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_variant_64 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					KernelX.Tool.PopCap.ZLib.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', { variant_64: version_variant_64 });
					return [`${ripe_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					raw_file: '?input',
					ripe_file: '?default',
					version_variant_64: configuration.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'popcap.zlib.uncompress',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file: Executor.Argument<string, false>;
					raw_file: Executor.Argument<string, true>;
					version_variant_64: Executor.Argument<boolean, false>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					let version_variant_64: boolean;
					{
						ripe_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file'),
							a.ripe_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_variant_64 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					KernelX.Tool.PopCap.ZLib.uncompress_fs(ripe_file, raw_file, 15n, { variant_64: version_variant_64 });
					return [`${raw_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					ripe_file: '?input',
					raw_file: '?default',
					version_variant_64: configuration.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.zlib.compress.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					raw_file_directory: Executor.Argument<string, false>;
					ripe_file_directory: Executor.Argument<string, true>;
					version_variant_64: Executor.Argument<boolean, false>;
				}) {
					let raw_file_directory: string;
					let ripe_file_directory: string;
					let version_variant_64: boolean;
					{
						raw_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						ripe_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file_directory'),
							a.ripe_file_directory,
							(value) => (value),
							() => (raw_file_directory.replace(/$/i, '.compress')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_variant_64 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					simple_batch_execute(
						raw_file_directory,
						['file', null],
						(item) => {
							let raw_file = `${raw_file_directory}/${item}`;
							let ripe_file = `${ripe_file_directory}/${item}`;
							KernelX.Tool.PopCap.ZLib.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', { variant_64: version_variant_64 });
						},
					);
					return [`${ripe_file_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					raw_file_directory: '?input',
					ripe_file_directory: '?default',
					version_variant_64: configuration.version_variant_64,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'raw_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.zlib.uncompress.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					ripe_file_directory: Executor.Argument<string, false>;
					raw_file_directory: Executor.Argument<string, true>;
					version_variant_64: Executor.Argument<boolean, false>;
				}) {
					let ripe_file_directory: string;
					let raw_file_directory: string;
					let version_variant_64: boolean;
					{
						ripe_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_file_directory'),
							a.ripe_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						raw_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_file_directory.replace(/$/i, '.uncompress')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_variant_64 = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							(initial) => (Console.confirmation(null, null, initial)),
						);
					}
					simple_batch_execute(
						ripe_file_directory,
						['file', null],
						(item) => {
							let ripe_file = `${ripe_file_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item}`;
							KernelX.Tool.PopCap.ZLib.uncompress_fs(ripe_file, raw_file, 15n, { variant_64: version_variant_64 });
						},
					);
					return [`${raw_file_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					ripe_file_directory: '?input',
					raw_file_directory: '?default',
					version_variant_64: configuration.version_variant_64,
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