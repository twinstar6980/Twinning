namespace TwinStar.Script.Executor.Implement.pvz2.remote_project {

	// ------------------------------------------------

	// execute

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('pvz2.remote_project', [
			typical_method({
				id: 'execute',
				filter: ['directory', /(\.pvz2_remote_project)$/i],
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
						option: Support.PvZ2.RemoteProject.ActionE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'target',
						option: Support.PvZ2.RemoteProject.TargetE,
						checker: null,
						automatic: null,
						condition: (argument: { action: string; }) => (['clear', 'pull', 'push'].includes(argument.action) ? null : ''),
						default: '?input',
					}),
				],
				worker: ({ project_directory, action, target }) => {
					Support.PvZ2.RemoteProject.execute(project_directory, action as any, target === '' ? null : target as any);
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

TwinStar.Script.Executor.Implement.pvz2.remote_project.injector;