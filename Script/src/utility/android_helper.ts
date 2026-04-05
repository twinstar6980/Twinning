namespace Twinning.Script.AndroidHelper {

	// #region common

	export const k_temporary_directory = new StoragePath(`/data/local/tmp/twinning`);

	// ----------------

	function escape(
		source: string,
	): string {
		return source.replaceAll(/(?=['" ])/g, `\\`);
	}

	function run_sh(
		argument: Array<string>,
	): string {
		let result = ProcessHelper.run_process(ProcessHelper.search_program_ensure('sh', true), argument, null);
		if (result.code !== 0n) {
			throw new Error(`sh execute failed: ${result.code}\n${result.output}\n${result.error}`);
		}
		return result.output;
	}

	function run_adb(
		argument: Array<string>,
	): string {
		let result = ProcessHelper.run_process(ProcessHelper.search_program_ensure('adb', true), argument, null);
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
		local: StoragePath,
		remote: StoragePath,
	): void {
		StorageHelper.remove_if(local);
		let local_parent = local.parent();
		if (local_parent !== null && !StorageHelper.exist_directory(local_parent)) {
			StorageHelper.create_directory(local_parent);
		}
		if (k_mode === 'native') {
			fs_copy(remote, local, false);
		}
		if (k_mode === 'bridge') {
			run_adb([`pull`, remote.emit(), local.emit()]);
		}
		return;
	}

	export function push(
		local: StoragePath,
		remote: StoragePath,
		owner: null | string,
	): void {
		fs_remove(remote);
		let remote_parent = remote.parent();
		if (remote_parent !== null) {
			fs_create_directory(remote_parent, null);
		}
		if (k_mode === 'native') {
			fs_copy(local, remote, false);
		}
		if (k_mode === 'bridge') {
			if (fs_is_fuse_media_path(remote)) {
				run_adb([`push`, local.emit(), remote.emit()]);
			}
			else {
				fs_create_directory(k_temporary_directory, null);
				let remote_temporary = k_temporary_directory.join(local.name() ?? '');
				run_adb([`push`, local.emit(), remote_temporary.emit()]);
				fs_copy(remote_temporary, remote, false);
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
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\//.test(target.emit());
	}

	export function fs_is_fuse_media_path(
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?!(Android\/(data|obb))(?=\/|$))/.test(target.emit());
	}

	export function fs_is_fuse_ext_path(
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(data|obb))(?=\/|$))/.test(target.emit());
	}

	export function fs_is_fuse_ext_data_path(
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(data))(?=\/|$))/.test(target.emit());
	}

	export function fs_is_fuse_ext_obb_path(
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(obb))(?=\/|$))/.test(target.emit());
	}

	// ----------------

	export function fs_exist(
		target: StoragePath,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -e ${escape(target.emit())} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_copy(
		target: StoragePath,
		placement: StoragePath,
		follow_link: boolean,
	): void {
		let shell_result: string;
		shell_result = shell(`cp -rf ${!follow_link ? '-P' : '-L'} ${escape(target.emit())} ${escape(placement.emit())}`);
		return;
	}

	export function fs_rename(
		target: StoragePath,
		placement: StoragePath,
	): void {
		let shell_result: string;
		shell_result = shell(`mv -f ${escape(target.emit())} ${escape(placement.emit())}`);
		return;
	}

	export function fs_remove(
		target: StoragePath,
	): void {
		let shell_result: string;
		shell_result = shell(`rm -rf ${escape(target.emit())}`);
		return;
	}

	export function fs_exist_file(
		target: StoragePath,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -f ${escape(target.emit())} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_exist_directory(
		target: StoragePath,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -d ${escape(target.emit())} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_create_directory(
		target: StoragePath,
		mode: null | string,
	): void {
		mode = CheckHelper.not_null_or(mode, '777');
		let shell_result: string;
		shell_result = shell(`mkdir -p -m ${escape(mode)} ${escape(target.emit())}`);
		return;
	}

	// ----------------

	export function fs_change_mode(
		target: StoragePath,
		mode: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chmod -R ${escape(mode)} ${escape(target.emit())}`);
		return;
	}

	export function fs_change_owner(
		target: StoragePath,
		owner: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chown -R ${escape(owner)} ${escape(target.emit())}`);
		return;
	}

	export function fs_change_group(
		target: StoragePath,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chgrp -R ${escape(group)} ${escape(target.emit())}`);
		return;
	}

	export function fs_change_owner_group(
		target: StoragePath,
		owner: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chown -R ${escape(owner)}:${escape(group)} ${escape(target.emit())}`);
		return;
	}

	export function fs_change_owner_group_fuse(
		target: StoragePath,
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
