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
				worker(a: Entry.CommonArgument & {
					project_directory: Executor.Argument<string, false>;
					action: Executor.Argument<string, false>;
				}) {
					let project_directory: string;
					let action: string;
					{
						project_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'project_directory'),
							a.project_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						action = Executor.request_argument(
							Executor.query_argument_name(this.id, 'action'),
							a.action,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_string(Support.PvZ2.RemoteAndroidHelper.ActionE), null, null, initial)),
						);
					}
					Support.PvZ2.RemoteAndroidHelper.execute(project_directory, action as any);
					return [`${project_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					project_directory: '?input',
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