namespace TwinStar.Script.Entry.method.pvz2.remote_android_helper {

	// ------------------------------------------------

	// launch

	type Configuration = {
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'pvz2.remote_android_helper.launch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					project_directory: Executor.RequireArgument<string>;
					action: Executor.RequestArgument<string, false>;
				}) {
					let project_directory: string;
					let action: string;
					{
						project_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'project_directory'),
							a.project_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						action = Executor.request_argument(
							Executor.query_argument_name(this.id, 'action'),
							a.action,
							(value) => (value),
							null,
							(initial) => (Console.option(Support.PvZ2.RemoteAndroidHelper.ActionE.map((e) => ([e])), null, null, initial)),
						);
					}
					Support.PvZ2.RemoteAndroidHelper.execute(project_directory, action as any);
					Console.message('s', los(`执行成功`), [`${project_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					project_directory: undefined!,
					action: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pvz2_remote_android_helper_project)$/i]]),
				input_forwarder: 'project_directory',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.pvz2.remote_android_helper._injector,
});