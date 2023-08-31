namespace TwinStar.Script.Executor.Implement.pvz2.remote_android_helper {

	// ------------------------------------------------

	// launch

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('pvz2.remote_android_helper', [
			typical_method({
				id: 'launch',
				filter: ['directory', /(\.pvz2_remote_android_helper_project)$/i],
				argument: [
					typical_argument_path({
						id: 'project_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'action',
						option: Support.PvZ2.RemoteAndroidHelper.ActionE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ project_directory, action }) => {
					Support.PvZ2.RemoteAndroidHelper.execute(project_directory, action as any);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.pvz2.remote_android_helper.injector;