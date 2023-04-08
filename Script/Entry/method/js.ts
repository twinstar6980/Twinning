namespace TwinStar.Script.Entry.method.js {

	// ------------------------------------------------

	// execute

	type Configuration = {
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'js.execute',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					script_file: Executor.RequireArgument<string>;
				}) {
					let script_file: string;
					{
						script_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'script_file'),
							a.script_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
					}
					let result = CoreX.Miscellaneous.evaluate_fs(script_file);
					Console.success(los(`执行成功`), [`${result}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					script_file: undefined!,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.js)$/i]]),
				input_forwarder: 'script_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.js._injector,
});