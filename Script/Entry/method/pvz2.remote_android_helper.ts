/**
 * + pvz2.remote_android_helper.launch
 */
namespace TwinStar.Script.Entry.method.pvz2.remote_android_helper {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'pvz2.remote_android_helper.launch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					project_directory: Executor.RequireArgument<string>;
					action: Executor.RequestArgument<string, false>;
				}) {
					let project_directory: string;
					let action: Support.PvZ2.RemoteAndroidHelper.Action;
					{
						project_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'project_directory'),
							a.project_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						action = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'action'),
							a.action,
							(value) => (value),
							null,
							() => (Console.option(Support.PvZ2.RemoteAndroidHelper.ActionE.map((e) => ([e])), null)),
							(value) => (Support.PvZ2.RemoteAndroidHelper.ActionE.includes(value as any) ? null : los(`未知操作`)),
						);
					}
					Support.PvZ2.RemoteAndroidHelper.execute(project_directory, action);
					Console.notify('s', los(`执行成功`), [`${project_directory}`]);
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
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.pvz2.remote_android_helper._injector,
});