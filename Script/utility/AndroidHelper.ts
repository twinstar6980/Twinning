namespace Twinning.Script.AndroidHelper {

	// ------------------------------------------------

	export let g_sh_program_file: null | string = null;

	export let g_adb_program_file: null | string = null;

	// ------------------------------------------------

	export const k_remote_temporary_directory = `/data/local/tmp/twinning`;

	function es(
		source: string,
	): string {
		return source.replaceAll(/(?=['" ])/g, `\\`);
	}

	function run_sh(
		argument: Array<string>,
	): string {
		let program_path = g_sh_program_file !== null ? g_sh_program_file : ProcessHelper.search_path(`sh`);
		assert_test(program_path !== null, `could not find 'sh' program from PATH environment`);
		let execute_result = ProcessHelper.execute(program_path, argument, KernelX.Process.list_environment_variable());
		if (execute_result.code !== 0n) {
			throw new Error(`sh execute failed : ${execute_result.code}\n${execute_result.output}\n${execute_result.error}`);
		}
		return execute_result.output;
	}

	function run_adb(
		argument: Array<string>,
	): string {
		let program_path = g_adb_program_file !== null ? g_adb_program_file : ProcessHelper.search_path(`adb`);
		assert_test(program_path !== null, `could not find 'adb' program from PATH environment`);
		let execute_result = ProcessHelper.execute(program_path, argument, KernelX.Process.list_environment_variable());
		if (execute_result.code !== 0n) {
			throw new Error(`adb execute failed : ${execute_result.code}\n${execute_result.output}\n${execute_result.error}`);
		}
		return execute_result.output;
	}

	// ------------------------------------------------

	let g_mode: 'native' | 'bridge' = KernelX.is_android ? 'native' : 'bridge';

	export function shell(
		command: string,
	): string {
		let result: string;
		if (g_mode === 'native') {
			result = run_sh([`-c`, `su -c "${command}"`]);
		}
		if (g_mode === 'bridge') {
			result = run_adb([`shell`, `su -c "${command}"`]);
		}
		return result!;
	}

	export function pull(
		local: string,
		remote: string,
	): void {
		KernelX.Storage.remove(local);
		let local_parent = PathUtility.parent(local);
		if (local_parent !== null) {
			KernelX.Storage.create_directory(local_parent);
		}
		if (g_mode === 'native') {
			fs_copy(remote, local);
		}
		if (g_mode === 'bridge') {
			run_adb([`pull`, remote, local]);
		}
		return;
	}

	export function push(
		local: string,
		remote: string,
		owner: null | string = null,
	): void {
		fs_remove(remote);
		let remote_parent = PathUtility.parent(remote);
		if (remote_parent !== null) {
			fs_create_directory(remote_parent);
		}
		if (g_mode === 'native') {
			fs_copy(local, remote);
		}
		if (g_mode === 'bridge') {
			if (fs_is_fuse_media_path(remote)) {
				run_adb([`push`, local, remote]);
			}
			else {
				fs_create_directory(k_remote_temporary_directory);
				let remote_temporary = `${k_remote_temporary_directory}/${PathUtility.name(local)}`;
				run_adb([`push`, local, remote_temporary]);
				fs_copy(remote_temporary, remote);
				fs_remove(remote_temporary);
			}
		}
		if (owner !== null && !fs_is_fuse_media_path(remote)) {
			fs_change_owner_group_fuse(remote, owner);
		}
		return;
	}

	// ------------------------------------------------

	export function fs_is_fuse_path(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\//.test(target);
	}

	export function fs_is_fuse_media_path(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?!(Android\/(data|obb))(?=\/|$))/.test(target);
	}

	export function fs_is_fuse_ext_path(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(data|obb))(?=\/|$))/.test(target);
	}

	export function fs_is_fuse_ext_data_path(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/data)(?=\/|$))/.test(target);
	}

	export function fs_is_fuse_ext_obb_path(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/obb)(?=\/|$))/.test(target);
	}

	// ------------------------------------------------

	export function fs_exist(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -e ${es(target)} ] ; then echo y ; else echo n ; fi`);
		return split_string_by_line_feed(shell_result)[0] === 'y';
	}

	export function fs_exist_file(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -f ${es(target)} ] ; then echo y ; else echo n ; fi`);
		return split_string_by_line_feed(shell_result)[0] === 'y';
	}

	export function fs_exist_directory(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -d ${es(target)} ] ; then echo y ; else echo n ; fi`);
		return split_string_by_line_feed(shell_result)[0] === 'y';
	}

	export function fs_copy(
		source: string,
		destination: string,
	): void {
		let shell_result: string;
		shell_result = shell(`cp -rf ${es(source)} ${es(destination)}`);
		return;
	}

	export function fs_rename(
		source: string,
		destination: string,
	): void {
		let shell_result: string;
		shell_result = shell(`mv -f ${es(source)} ${es(destination)}`);
		return;
	}

	export function fs_remove(
		target: string,
	): void {
		let shell_result: string;
		shell_result = shell(`rm -rf ${es(target)}`);
		return;
	}

	export function fs_create_directory(
		target: string,
		mode: string = '777',
	): void {
		let shell_result: string;
		shell_result = shell(`mkdir -p -m ${es(mode)} ${es(target)}`);
		return;
	}

	// ------------------------------------------------

	export function fs_change_mode(
		target: string,
		mode: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chmod -R ${es(mode)} ${es(target)}`);
		return;
	}

	export function fs_change_owner(
		target: string,
		owner: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chown -R ${es(owner)} ${es(target)}`);
		return;
	}

	export function fs_change_group(
		target: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chgrp -R ${es(group)} ${es(target)}`);
		return;
	}

	export function fs_change_owner_group(
		target: string,
		owner: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chown -R ${es(owner)}:${es(group)} ${es(target)}`);
		return;
	}

	export function fs_change_owner_group_fuse(
		target: string,
		owner: string,
	): void {
		if (fs_is_fuse_path(target)) {
			if (fs_is_fuse_media_path(target)) {
				fs_change_owner_group(target, owner, 'media_rw');
			}
			if (fs_is_fuse_ext_data_path(target)) {
				fs_change_owner_group(target, owner, 'ext_data_rw');
			}
			if (fs_is_fuse_ext_obb_path(target)) {
				fs_change_owner_group(target, owner, 'ext_obb_rw');
			}
		}
		return;
	}

	// ------------------------------------------------

	export type ApplicationInformation = {
		id: string;
		version_code: bigint;
		version_name: string;
		user: string;
	};

	export function application_find(
		rule: RegExp,
	): Array<string> {
		let shell_result: string;
		shell_result = shell(`pm list packages`);
		let id = split_string_by_line_feed(shell_result).map((value) => (value.slice(8))).filter((value) => (rule.test(value)));
		return id;
	}

	export function application_query(
		application: string,
	): ApplicationInformation {
		let shell_result: string;
		let match: null | RegExpMatchArray;
		let result: ApplicationInformation = {} as any;
		result.id = application;
		shell_result = shell(`pm dump ${es(application)}`);
		match = /versionCode=([0-9]+)/.exec(shell_result);
		assert_test(match !== null);
		result.version_code = BigInt(match[1]);
		match = /versionName=([0-9a-zA-Z.+-]+)/.exec(shell_result);
		assert_test(match !== null);
		result.version_name = match[1];
		match = /userId=([0-9]+)/.exec(shell_result);
		assert_test(match !== null);
		let user_number = BigInt(match[1]);
		result.user = `u${user_number / 100000n}_a${user_number - 10000n}`;
		return result;
	}

	export function application_start(
		application: string,
		activity: string,
	): void {
		let shell_result: string;
		shell_result = shell(`am start -n ${es(application)}/${es(activity)}`);
		return;
	}

	// ------------------------------------------------

}