namespace Twinning.Script.Executor.Implement.pvz2.package_project {

	// ------------------------------------------------

	// transpile
	// compile
	// link
	// parse

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
		default_conversion_setting: Support.PvZ2.PackageProject.ConversionSetting;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'pvz2.package_project', [
			typical_method({
				id: 'transpile',
				filter: 'directory',
				argument: [
					typical_argument_path({
						id: 'project_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'target_scope',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'target_package',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'option_generalize_rton',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'option_generalize_ptx',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'option_generalize_pam',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'option_generalize_wem',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'option_specialize_rton',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'option_specialize_ptx',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'option_specialize_pam',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'option_specialize_wem',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({
					project_directory,
					target_scope,
					target_package,
					option_generalize_rton,
					option_generalize_ptx,
					option_generalize_pam,
					option_generalize_wem,
					option_specialize_rton,
					option_specialize_ptx,
					option_specialize_pam,
					option_specialize_wem,
					buffer_size,
				}) => {
					let target_scope_list = target_scope === '*' ? null : target_scope.length === 0 ? [] : target_scope.split('|').map(Support.PvZ2.PackageProject.parse_scope_expression);
					let option: Support.PvZ2.PackageProject.Transpile.Option = {
						generalize_rton: option_generalize_rton,
						generalize_ptx: option_generalize_ptx,
						generalize_pam: option_generalize_pam,
						generalize_wem: option_generalize_wem,
						specialize_rton: option_specialize_rton,
						specialize_ptx: option_specialize_ptx,
						specialize_pam: option_specialize_pam,
						specialize_wem: option_specialize_wem,
					};
					Support.PvZ2.PackageProject.Transpile.transpile(project_directory, target_scope_list as any, target_package, option, buffer_size);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'compile',
				filter: 'directory',
				argument: [
					typical_argument_path({
						id: 'project_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'target_scope',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'target_package',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ project_directory, target_scope, target_package, buffer_size }) => {
					let target_scope_list = target_scope === '*' ? null : target_scope.length === 0 ? [] : target_scope.split('|').map(Support.PvZ2.PackageProject.parse_scope_expression);
					let target_package_list = target_package === '*' ? null : target_package.length === 0 ? [] : target_package.split('|');
					Support.PvZ2.PackageProject.Compile.compile(project_directory, target_scope_list as any, target_package_list, buffer_size);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'link',
				filter: 'directory',
				argument: [
					typical_argument_path({
						id: 'project_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'target_package',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'remake_manifest',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ project_directory, target_package, remake_manifest, buffer_size }) => {
					let target_package_list = target_package === '*' ? null : target_package.length === 0 ? [] : target_package.split('|');
					Support.PvZ2.PackageProject.Link.link(project_directory, target_package_list, remake_manifest, buffer_size);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'parse',
				filter: 'directory',
				argument: [
					typical_argument_path({
						id: 'project_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'package_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						id: 'package_name',
						option: null,
						checker: null,
						automatic: (argument: { package_directory: string; }) => (PathUtility.name(argument.package_directory).replace(/(\.rsb\.bundle)?$/i, '')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'package_version_number',
						option: KernelX.Tool.PopCap.ResourceStreamBundle.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_integer({
						id: 'package_version_extended_texture_information_for_pvz2_cn',
						option: KernelX.Tool.PopCap.ResourceStreamBundle.VersionaExtendedTextureInformationForPVZ2CNE,
						checker: null,
						automatic: null,
						condition: (argument: { package_version_number: bigint; }) => ([4n].includes(argument.package_version_number) ? null : 0n),
					}),
				],
				worker: ({ project_directory, package_directory, package_name, package_version_number, package_version_extended_texture_information_for_pvz2_cn }) => {
					Support.PvZ2.PackageProject.Parse.parse(project_directory, package_directory, package_name, { number: package_version_number as any, extended_texture_information_for_pvz2_cn: package_version_extended_texture_information_for_pvz2_cn as any }, configuration.default_conversion_setting);
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

Twinning.Script.Executor.Implement.pvz2.package_project.injector;