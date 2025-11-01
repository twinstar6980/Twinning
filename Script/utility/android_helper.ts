namespace Twinning.Script.AndroidHelper {

	// #region common

	export const k_temporary_directory = `/data/local/tmp/twinning`;

	// ----------------

	function escape(
		source: string,
	): string {
		return source.replaceAll(/(?=['" ])/g, `\\`);
	}

	function run_sh(
		argument: Array<string>,
	): string {
		let result = ProcessHelper.run_process(['sh'], argument, null, null);
		if (result.code !== 0n) {
			throw new Error(`sh execute failed: ${result.code}\n${result.output}\n${result.error}`);
		}
		return result.output;
	}

	function run_adb(
		argument: Array<string>,
	): string {
		let result = ProcessHelper.run_process(['adb'], argument, null, null);
		if (result.code !== 0n) {
			throw new Error(`adb execute failed: ${result.code}\n${result.output}\n${result.error}`);
		}
		return result.output;
	}

	// #endregion

	// #region basic

	const k_mode = KernelX.is_android ? 'native' : 'bridge';

	// ----------------

	export function shell(
		command: string,
	): string {
		let result: string;
		if (k_mode === 'native') {
			result = run_sh([`-c`, `su -c "${command}"`]);
		}
		if (k_mode === 'bridge') {
			result = run_adb([`shell`, `su -c "${command}"`]);
		}
		return result!;
	}

	export function pull(
		local: string,
		remote: string,
	): void {
		KernelX.Storage.remove_if(local);
		let local_parent = StorageHelper.parent(local);
		if (local_parent !== null) {
			KernelX.Storage.create_directory(local_parent);
		}
		if (k_mode === 'native') {
			fs_copy(remote, local);
		}
		if (k_mode === 'bridge') {
			run_adb([`pull`, remote, local]);
		}
		return;
	}

	export function push(
		local: string,
		remote: string,
		owner: null | string,
	): void {
		fs_remove(remote);
		let remote_parent = StorageHelper.parent(remote);
		if (remote_parent !== null) {
			fs_create_directory(remote_parent, null);
		}
		if (k_mode === 'native') {
			fs_copy(local, remote);
		}
		if (k_mode === 'bridge') {
			if (fs_is_fuse_media_path(remote)) {
				run_adb([`push`, local, remote]);
			}
			else {
				fs_create_directory(k_temporary_directory, null);
				let remote_temporary = `${k_temporary_directory}/${StorageHelper.name(local)}`;
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

	// #endregion

	// #region storage

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
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(data))(?=\/|$))/.test(target);
	}

	export function fs_is_fuse_ext_obb_path(
		target: string,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(obb))(?=\/|$))/.test(target);
	}

	// ----------------

	export function fs_exist(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -e ${escape(target)} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_exist_file(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -f ${escape(target)} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_exist_directory(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -d ${escape(target)} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_copy(
		source: string,
		destination: string,
	): void {
		let shell_result: string;
		shell_result = shell(`cp -rf ${escape(source)} ${escape(destination)}`);
		return;
	}

	export function fs_rename(
		source: string,
		destination: string,
	): void {
		let shell_result: string;
		shell_result = shell(`mv -f ${escape(source)} ${escape(destination)}`);
		return;
	}

	export function fs_remove(
		target: string,
	): void {
		let shell_result: string;
		shell_result = shell(`rm -rf ${escape(target)}`);
		return;
	}

	export function fs_create_directory(
		target: string,
		mode: null | string,
	): void {
		mode = CheckHelper.not_null_or(mode, '777');
		let shell_result: string;
		shell_result = shell(`mkdir -p -m ${escape(mode)} ${escape(target)}`);
		return;
	}

	// ----------------

	export function fs_change_mode(
		target: string,
		mode: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chmod -R ${escape(mode)} ${escape(target)}`);
		return;
	}

	export function fs_change_owner(
		target: string,
		owner: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chown -R ${escape(owner)} ${escape(target)}`);
		return;
	}

	export function fs_change_group(
		target: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chgrp -R ${escape(group)} ${escape(target)}`);
		return;
	}

	export function fs_change_owner_group(
		target: string,
		owner: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chown -R ${escape(owner)}:${escape(group)} ${escape(target)}`);
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

	// #endregion

	// #region application

	export type ApplicationInformation = {
		identifier: string;
		version_code: bigint;
		version_name: string;
		user: string;
	};

	// ----------------

	export function application_find(
		rule: RegExp,
	): Array<string> {
		let shell_result: string;
		shell_result = shell(`pm list packages`);
		let result = ConvertHelper.split_string_by_line_feed(shell_result, true).map((value) => (value.slice(8))).filter((value) => (rule.test(value)));
		return result;
	}

	export function application_query(
		application: string,
	): ApplicationInformation {
		let shell_result: string;
		let match: null | RegExpMatchArray;
		let result: ApplicationInformation = {} as any;
		result.identifier = application;
		shell_result = shell(`pm dump ${escape(application)}`);
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
		shell_result = shell(`am start -n ${escape(application)}/${escape(activity)}`);
		return;
	}

	// #endregion

}
