namespace TwinStar.Script.Entry.method.data.differentiation {

	// ------------------------------------------------

	// vcdiff.encode
	// vcdiff.decode

	type Configuration = {
		encode_buffer_size: Executor.Argument<string, false>;
		decode_buffer_size: Executor.Argument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.differentiation.vcdiff.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					before_file: Executor.Argument<string, false>;
					after_file: Executor.Argument<string, false>;
					patch_file: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let before_file: string;
					let after_file: string;
					let patch_file: string;
					let buffer_size: bigint;
					{
						before_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'before_file'),
							a.before_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						after_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'after_file'),
							a.after_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						patch_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'patch_file'),
							a.patch_file,
							(value) => (value),
							() => (after_file.replace(/()?$/i, '.patch.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.Data.Differentiation.VCDiff.encode_fs(before_file, after_file, patch_file, false, buffer_size);
					return [`${patch_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					before_file: undefined!,
					after_file: '?input',
					patch_file: '?default',
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'before_file',
			}),
			Executor.method_of({
				id: 'data.differentiation.vcdiff.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					before_file: Executor.Argument<string, false>;
					patch_file: Executor.Argument<string, false>;
					after_file: Executor.Argument<string, true>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let before_file: string;
					let patch_file: string;
					let after_file: string;
					let buffer_size: bigint;
					{
						before_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'before_file'),
							a.before_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						patch_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'patch_file'),
							a.patch_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						after_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'after_file'),
							a.after_file,
							(value) => (value),
							() => (patch_file.replace(/()?$/i, '.after.bin')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.Data.Differentiation.VCDiff.decode_fs(before_file, after_file, patch_file, 0x7FFFFFFFn, buffer_size);
					return [`${after_file}`];
				},
				default_argument: {
					...Entry.k_common_argument,
					before_file: undefined!,
					patch_file: '?input',
					after_file: '?default',
					buffer_size: configuration.decode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'before_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.data.differentiation._injector,
});