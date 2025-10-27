namespace Twinning.Script.Executor.Implement.PvZ2.RemoteProject {

	// #region interface

	export function activate(
	): void {
		push_typical_method('pvz2.remote_project', [
			typical_method({
				identifier: 'execute',
				filter: ['directory', /(\.pvz2_remote_project)$/i],
				argument: [
					typical_argument_path({
						identifier: 'project_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'action',
						option: Support.PvZ2.RemoteProject.ActionE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'target',
						option: Support.PvZ2.RemoteProject.TargetE,
						checker: null,
						automatic: null,
						condition: (argument: { action: string; }) => (['clear', 'pull', 'push'].includes(argument.action) ? null : ''),
					}),
					typical_argument_integer({
						identifier: 'rton_version_number',
						option: KernelX.Tool.PopCap.ReflectionObjectNotation.VersionNumberE,
						checker: null,
						automatic: null,
						condition: (argument: { action: string; target: string; }) => (['pull', 'push'].includes(argument.action) && ['content_delivery', 'local_profile', 'player_profile'].includes(argument.target) ? null : 0n),
					}),
					typical_argument_boolean({
						identifier: 'rton_version_native_string_encoding_use_utf8',
						checker: null,
						automatic: null,
						condition: (argument: { action: string; target: string; }) => (['pull', 'push'].includes(argument.action) && ['content_delivery', 'local_profile', 'player_profile'].includes(argument.target) ? null : false),
					}),
					typical_argument_size({
						identifier: 'rton_encode_buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: (argument: { action: string; target: string; }) => (['push'].includes(argument.action) && ['content_delivery', 'local_profile', 'player_profile'].includes(argument.target) ? null : 0n),
					}),
				],
				batch: null,
				worker: ({ project_directory, action, target, rton_version_number, rton_version_native_string_encoding_use_utf8, rton_encode_buffer_size }, temporary: {}) => {
					Support.PvZ2.RemoteProject.execute(project_directory, action as any, target === '' ? null : target as any, { number: rton_version_number as any, native_string_encoding_use_utf8: rton_version_native_string_encoding_use_utf8 }, rton_encode_buffer_size);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
