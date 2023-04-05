namespace TwinStar.Script.Entry.method.data.hash {

	// ------------------------------------------------

	// md5

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.hash.md5',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					target_file: Executor.RequireArgument<string>;
				}) {
					let target_file: string;
					{
						target_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'target_file'),
							a.target_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
					}
					let result = CoreX.Tool.Data.Hash.MD5.hash_fs(target_file);
					Console.message('s', los(`执行成功`), [`${result.toString(16)}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					target_file: undefined!,
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