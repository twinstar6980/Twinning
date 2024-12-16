namespace Twinning.Script.Support.PvZ2.PackageProject {

	// ------------------------------------------------

	export const k_version = 2n;

	// ------------------------------------------------

	export type Scope0 = [];

	export type Scope1 = [string];

	export type Scope2 = [string, string];

	export type Scope3 = [string, string, string];

	export type Scope = [] | [string] | [string, string] | [string, string, string];

	export function parse_scope_expression(
		source: string,
	): Scope {
		let destination = source.split('/');
		assert_test(destination[0] === '');
		assert_test(destination.length <= 4);
		destination = destination.slice(1);
		return destination as Scope;
	}

	// ------------------------------------------------

	export type ResourceState = Array<{
		category: RegularResourceManifest.SubgroupCategory;
		definition: null | Kernel.Tool.PopCap.ResourceStreamBundle.Definition.JS_N.Resource;
		manifest: RegularResourceManifest.Resource;
	}>;

	export type GroupState = {
		id: string;
		resource: Record<string, ResourceState>;
	};

	export type PartState = {
		group: Record<string, GroupState>;
	};

	export type PackageState = {
		part: Record<string, PartState>;
	};

	// ------------------------------------------------

	export function query_state_of_resource(
		group_state: GroupState,
		resource_name: string,
	): ResourceState {
		let resource_state: ResourceState = [
		];
		group_state.resource[resource_name] = resource_state;
		return resource_state;
	}

	export function query_state_of_group(
		part_state: PartState,
		group_name: string,
	): GroupState {
		let group_state = part_state.group[group_name];
		if (group_state === undefined) {
			group_state = {
				id: '',
				resource: {
				},
			};
			part_state.group[group_name] = group_state;
		}
		return group_state;
	}

	export function query_state_of_part(
		package_state: PackageState,
		part_name: string,
	): PartState {
		let part_state = package_state.part[part_name];
		if (part_state === undefined) {
			part_state = {
				group: {
				},
			};
			package_state.part[part_name] = part_state;
		}
		return part_state;
	}

	// ------------------------------------------------

	export function merge_variable_list(
		parent_list: Array<Variable>,
		child_list: Array<Variable>,
	): Array<Variable> {
		return [
			...child_list,
			...parent_list,
		];
	}

	export function find_variable_value(
		list: Array<Variable>,
		name: string,
	): null | string {
		let result = list.find((value) => (value.name === name));
		return result === undefined ? null : parse_variable_string(result.value, list);
	}

	export function parse_variable_string(
		format: string,
		variable_list: Array<Variable>,
	): string {
		let result = '';
		let variable_name: null | string = null;
		format = format.slice(0, not_or(format.indexOf('~'), -1, undefined));
		for (let index = 0; index < format.length; index++) {
			let character = format[index];
			if (character === '{') {
				assert_test(variable_name === null);
				variable_name = '';
				continue;
			}
			if (character === '}') {
				assert_test(variable_name !== null);
				let variable_value = find_variable_value(variable_list, variable_name);
				assert_test(variable_value !== null, `could not find variable '${variable_name}'`);
				result += variable_value;
				variable_name = null;
				continue;
			}
			if (variable_name !== null) {
				variable_name += character;
			}
			else {
				result += character;
			}
		}
		return result;
	}

	// ------------------------------------------------

	export function make_subgroup_name(
		group_name: string,
		subgroup_category: RegularResourceManifest.SubgroupCategory,
	): string {
		let subgroup_name = group_name;
		if (subgroup_category.resolution !== null) {
			subgroup_name += `_${subgroup_category.resolution}`;
		}
		if (subgroup_category.locale !== null) {
			subgroup_name += `_${subgroup_category.locale}`;
		}
		if (subgroup_category.resolution === null && subgroup_category.locale === null) {
			subgroup_name += `_Common`;
		}
		return subgroup_name;
	}

	// ------------------------------------------------

	export function find_conversion_setting<Type extends keyof ConversionSetting>(
		list: ConversionSetting,
		type: Type,
		name: string,
	): null | ConversionSetting[Type][number] {
		let result = list[type].find((value) => (value.name === name));
		return result === undefined ? null : result;
	}

	export function find_conversion_setting_strict<Type extends keyof ConversionSetting>(
		list: ConversionSetting,
		type: Type,
		name: string,
	): ConversionSetting[Type][number] {
		let result = find_conversion_setting(list, type, name);
		if (result === null) {
			throw new Error(`could not find conversion setting of type '${type}' by name '${name}'`);
		}
		return result;
	}

	// ------------------------------------------------

	export function list_scope_child_name(
		parent_directory: string,
		child_scope: Scope,
	): Array<string> {
		return (child_scope.length !== 0 ? [child_scope[0]] : KernelX.Storage.list_directory(parent_directory, 1n)).filter((value) => (value.length !== 0 && !value.startsWith('.')));
	}

	export function make_scope_root_path(
		project_directory: string,
		part_name?: string,
		group_name?: string,
		resource_name?: string,
	): string {
		let result = project_directory;
		if (part_name !== undefined) {
			result += `/${part_name}`;
		}
		if (group_name !== undefined) {
			result += `/${group_name}`;
		}
		if (resource_name !== undefined) {
			result += `/${resource_name}`;
		}
		return result;
	}

	export function make_scope_child_path(
		parent_directory: string,
		child_name: string,
	): string {
		return `${parent_directory}/${child_name}`;
	}

	export function make_scope_setting_path(
		root_directory: string,
	): string {
		return `${root_directory}/setting.json`;
	}

	export function make_build_package_state_path(
		project_directory: string,
		package_name: string,
	): string {
		return `${project_directory}/.build/${package_name}/state.json`;
	}

	export function make_build_package_bundle_path(
		project_directory: string,
		package_name: string,
	): string {
		return `${project_directory}/.build/${package_name}/package.rsb.bundle`;
	}

	export function make_build_package_bundle_packet_path(
		project_directory: string,
		package_name: string,
		group_name: string,
		subgroup_category: RegularResourceManifest.SubgroupCategory,
	): string {
		return `${project_directory}/.build/${package_name}/package.rsb.bundle/packet/${make_subgroup_name(group_name, subgroup_category)}.rsg`;
	}

	export function make_build_package_bundle_resource_path(
		project_directory: string,
		package_name: string,
		resource_path: string,
	): string {
		return `${project_directory}/.build/${package_name}/package.rsb.bundle/resource/${resource_path}`;
	}

	export function make_build_package_data_path(
		project_directory: string,
		package_name: string,
	): string {
		return `${project_directory}/.build/${package_name}.rsb`;
	}

	// ------------------------------------------------

	export function check_version_file(
		project_directory: string,
	): void {
		try {
			let version_file = make_scope_child_path(project_directory, 'version.txt');
			let version_data = KernelX.Storage.read_file(version_file);
			let version_text = Kernel.Miscellaneous.cast_CharacterListView_to_JS_String(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(version_data.view()));
			assert_test(version_text === k_version.toString());
		}
		catch (e) {
			throw new Error(`failed to check version.txt, excepted '${k_version}'`);
		}
		return;
	}

	// ------------------------------------------------

}