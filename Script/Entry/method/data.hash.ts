namespace TwinStar.Script.Entry.method.data.hash {

	// ------------------------------------------------

	// md5

	type Configuration = {
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.hash.md5',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					target_file: Executor.Argument<string, false>;
				}) {
					let target_file: string;
					{
						target_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'target_file'),
							a.target_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
					}
					let result = KernelX.Tool.Data.Hash.MD5.hash_fs(target_file);
					return [`${result.toString(16)}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					target_file: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'target_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.data.hash._injector,
});