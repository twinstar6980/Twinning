namespace TwinStar.Script.Executor.Implement.pvz2.package_project {

	// ------------------------------------------------

	// compile
	// link
	// parse

	export type Configuration = {
		compile_buffer_size: TypicalArgumentExpression<string>;
		link_buffer_size: TypicalArgumentExpression<string>;
		parse_package_version_number: TypicalArgumentExpression<bigint>;
		parse_package_version_extended_texture_information_for_pvz2_cn: TypicalArgumentExpression<bigint>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('pvz2.package_project', [
			typical_method({
				id: 'compile',
				filter: ['directory', /(\.pvz2_package_project)$/i],
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
						id: 'target_scope',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'target_package',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.compile_buffer_size,
					}),
				],
				worker: ({ project_directory, target_scope, target_package, buffer_size }) => {
					let target_scope_list = target_scope === '*' ? null : target_scope.length === 0 ? [] : target_scope.split('|').map((value) => (Support.PvZ2.PackageProject.parse_scope_expression(value)));
					let target_package_list = target_package === '*' ? null : target_package.length === 0 ? [] : target_package.split('|');
					Support.PvZ2.PackageProject.Compile.compile(project_directory, target_scope_list as any, target_package_list, buffer_size);
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'link',
				filter: ['directory', /(\.pvz2_package_project)$/i],
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
						id: 'target_package',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_boolean({
						id: 'remake_manifest',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_size({
						id: 'buffer_size',
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.link_buffer_size,
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
				filter: ['directory', /(\.pvz2_package_project)$/i],
				argument: [
					typical_argument_path({
						id: 'project_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'package_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_string({
						id: 'package_name',
						option: null,
						checker: null,
						automatic: (argument: { package_directory: string; }) => (PathUtility.name(argument.package_directory).replace(/(\.rsb\.bundle)?$/i, '')) ,
						condition: null,
						default: '?automatic',
					}),
					typical_argument_integer({
						id: 'package_version_number',
						option: KernelX.Tool.PopCap.ResourceStreamBundle.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.parse_package_version_number,
					}),
					typical_argument_integer({
						id: 'package_version_extended_texture_information_for_pvz2_cn',
						option: KernelX.Tool.PopCap.ResourceStreamBundle.VersionaExtendedTextureInformationForPVZ2CNE,
						checker: null,
						automatic: null,
						condition: (argument: { package_version_number: bigint; }) => ([4n].includes(argument.package_version_number) ? null : 0n),
						default: configuration.parse_package_version_extended_texture_information_for_pvz2_cn,
					}),
				],
				worker: ({ project_directory, package_directory, package_name, package_version_number, package_version_extended_texture_information_for_pvz2_cn }) => {
					Support.PvZ2.PackageProject.Parse.parse(project_directory, package_directory, package_name, { number: package_version_number as any, extended_texture_information_for_pvz2_cn: package_version_extended_texture_information_for_pvz2_cn as any });
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

TwinStar.Script.Executor.Implement.pvz2.package_project.injector;