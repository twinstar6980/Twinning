namespace Twinning.Script.AndroidHelper {

	// #region common

	export let g_temporary_directory_for_data: StoragePath | null = null;

	export let g_temporary_directory_for_sdcard: StoragePath | null = null;

	// ----------------

	export function make_temporary_directory(
		location: 'data' | 'sdcard',
	): StoragePath {
		let path = null as StoragePath | null;
		if (location === 'data') {
			assert_test(g_temporary_directory_for_data !== null);
			path = g_temporary_directory_for_data;
			fs_create_directory(path, null);
			fs_ensure_public_access([path]);
		}
		if (location === 'sdcard') {
			assert_test(g_temporary_directory_for_sdcard !== null);
			path = g_temporary_directory_for_sdcard;
			if (!StorageHelper.exist_directory(path)) {
				StorageHelper.create_directory(path);
			}
		}
		assert_test(path !== null);
		return path;
	}

	// #endregion

	// #region basic

	const k_mode = KernelX.is_android ? 'native' : 'bridge';

	// ----------------

	export function shell(
		run_as_root: boolean | 'only_native',
		command: Array<Array<string | [string]>>,
	): string {
		let result: string;
		if (run_as_root === 'only_native') {
			run_as_root = k_mode === 'native';
		}
		let interpreter = !run_as_root ? 'sh' : 'su';
		let command_string = `${interpreter} -c ${ProcessHelper.escape_shell_string('sh', ProcessHelper.build_shell_script('sh', command))}`;
		if (k_mode === 'native') {
			result = ExternalHelper.run_sh_command(command_string, {host: 'native'});
		}
		if (k_mode === 'bridge') {
			result = ExternalHelper.run_adb_shell(command_string);
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
			if (fs_is_fuse_path(remote)) {
				ExternalHelper.run_adb_pull(remote, local);
			}
			else {
				let remote_temporary = make_temporary_directory('data').join(local.name() ?? '');
				fs_copy(remote, remote_temporary, false);
				fs_ensure_public_access([remote_temporary]);
				ExternalHelper.run_adb_pull(remote_temporary, local);
				fs_remove(remote_temporary);
			}
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
			if (fs_is_fuse_path(remote)) {
				ExternalHelper.run_adb_push(remote, local);
			}
			else {
				let remote_temporary = make_temporary_directory('data').join(local.name() ?? '');
				ExternalHelper.run_adb_push(remote_temporary, local);
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
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\//.test(target.emit_posix(true));
	}

	export function fs_is_fuse_media_path(
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?!(Android\/(data|obb))(?=\/|$))/.test(target.emit_posix(true));
	}

	export function fs_is_fuse_ext_path(
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(data|obb))(?=\/|$))/.test(target.emit_posix(true));
	}

	export function fs_is_fuse_ext_data_path(
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(data))(?=\/|$))/.test(target.emit_posix(true));
	}

	export function fs_is_fuse_ext_obb_path(
		target: StoragePath,
	): boolean {
		return /^\/((storage\/emulated\/[0-9]+)|(sdcard))\/(?=(Android\/(obb))(?=\/|$))/.test(target.emit_posix(true));
	}

	// ----------------

	export function fs_exist(
		target: StoragePath,
	): boolean {
		let shell_result: string;
		shell_result = shell(true, [[
			`if [`, `-e`, [`${target.emit_posix(true)}`], `];`,
			`then echo y;`,
			`else echo n;`,
			`fi`,
		]]);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_copy(
		target: StoragePath,
		placement: StoragePath,
		follow_link: boolean,
	): void {
		let shell_result: string;
		shell_result = shell(true, [[
			`cp`,
			`-rf`,
			`${!follow_link ? '-P' : '-L'}`,
			[`${target.emit_posix(true)}`],
			[`${placement.emit_posix(true)}`],
		]]);
		return;
	}

	export function fs_rename(
		target: StoragePath,
		placement: StoragePath,
	): void {
		let shell_result: string;
		shell_result = shell(true, [[
			`mv`,
			`-f`,
			[`${target.emit_posix(true)}`],
			[`${placement.emit_posix(true)}`],
		]]);
		return;
	}

	export function fs_remove(
		target: StoragePath,
	): void {
		let shell_result: string;
		shell_result = shell(true, [[
			`rm`,
			`-rf`,
			[`${target.emit_posix(true)}`],
		]]);
		return;
	}

	// ----------------

	export function fs_exist_file(
		target: StoragePath,
	): boolean {
		let shell_result: string;
		shell_result = shell(true, [[
			`if [`, `-f`, [`${target.emit_posix(true)}`], `];`,
			`then echo y;`,
			`else echo n;`,
			`fi`,
		]]);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	// ----------------

	export function fs_exist_directory(
		target: StoragePath,
	): boolean {
		let shell_result: string;
		shell_result = shell(true, [[
			`if [`, `-d`, [`${target.emit_posix(true)}`], `];`,
			`then echo y;`,
			`else echo n;`,
			`fi`,
		]]);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_create_directory(
		target: StoragePath,
		mode: null | string,
	): void {
		mode = CheckHelper.not_null_or(mode, '777');
		let shell_result: string;
		shell_result = shell(true, [[
			`mkdir`,
			`-p`,
			`-m`, [`${mode}`],
			[`${target.emit_posix(true)}`],
		]]);
		return;
	}

	// ----------------

	export function fs_change_mode(
		target: StoragePath,
		mode: string,
	): void {
		let shell_result: string;
		shell_result = shell(true, [[
			`chmod`,
			`-R`,
			[`${mode}`],
			[`${target.emit_posix(true)}`],
		]]);
		return;
	}

	export function fs_change_owner(
		target: StoragePath,
		owner: string,
	): void {
		let shell_result: string;
		shell_result = shell(true, [[
			`chown`,
			`-R`,
			[`${owner}`],
			[`${target.emit_posix(true)}`],
		]]);
		return;
	}

	export function fs_change_group(
		target: StoragePath,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(true, [[
			`chgrp`,
			`-R`,
			[`${group}`],
			[`${target.emit_posix(true)}`],
		]]);
		return;
	}

	export function fs_change_owner_group(
		target: StoragePath,
		owner: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(true, [[
			`chown`,
			`-R`,
			[`${owner}:${group}`],
			[`${target.emit_posix(true)}`],
		]]);
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

	// ----------------

	export function fs_ensure_public_access(
		target: Array<StoragePath>,
	): void {
		for (let target_item of target) {
			fs_change_mode(target_item, '777');
			fs_change_owner_group(target_item, 'root', 'root');
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
		shell_result = shell('only_native', [[
			`pm`,
			`list`, `packages`,
		]]);
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
		shell_result = shell('only_native', [[
			`pm`,
			`list`, `packages`,
			`-U`,
			[`${application}`],
		]]);
		{
			match = new RegExp(`^package:${application.replaceAll('.', '\.')} uid\:([0-9]+)$`, 'm').exec(shell_result);
			assert_test(match !== null);
			let user_number = BigInt(match[1]);
			result.user = `u${user_number / 100000n}_a${user_number - 10000n}`;
		}
		shell_result = shell('only_native', [[
			`pm`,
			`dump`,
			[`${application}`],
		]]);
		{
			match = /versionCode=([0-9]+)/.exec(shell_result);
			assert_test(match !== null);
			result.version_code = BigInt(match[1]);
		}
		{
			match = /versionName=([0-9a-zA-Z.+-]+)/.exec(shell_result);
			assert_test(match !== null);
			result.version_name = match[1];
		}
		return result;
	}

	export function application_start(
		application: string,
		activity: string,
		action: null | string,
		extra: Record<string, boolean | bigint | number | string>,
	): void {
		let shell_result: string;
		shell_result = shell('only_native', [[
			`am`,
			`start`,
			`--user`, `0`,
			`-n`, [`${application}/${activity}`],
			...(action === null ? [] : [
				`-a`, [`${action}`] as [string],
			]),
			...convert_to_extra_argument_list(extra).map((it) => [it] as [string]),
		]]);
		return;
	}

	export function application_start_service(
		application: string,
		service: string,
		action: null | string,
		extra: Record<string, boolean | bigint | number | string>,
		use_root: boolean,
	): void {
		let shell_result: string;
		shell_result = shell(use_root, [[
			`am`,
			`startservice`,
			`--user`, `0`,
			`-n`, [`${application}/${service}`],
			...(action === null ? [] : [
				`-a`, [`${action}`] as [string],
			]),
			...convert_to_extra_argument_list(extra).map((it) => [it] as [string]),
		]]);
		return;
	}

	// ----------------

	function convert_to_extra_argument_list(
		source: Record<string, boolean | bigint | number | string>,
	): Array<string> {
		let destination = [] as Array<string>;
		for (let key in source) {
			let value = source[key];
			let type = null as null | string;
			if (CheckHelper.is_boolean(value)) {
				type = 'z';
			}
			if (CheckHelper.is_bigint(value)) {
				type = 'l';
			}
			if (CheckHelper.is_number(value)) {
				type = 'f';
			}
			if (CheckHelper.is_string(value)) {
				type = 's';
			}
			assert_test(type !== null);
			destination.push(`--e${type}`);
			destination.push(`${key}`);
			destination.push(`${value}`);
		}
		return destination;
	}

	// #endregion

}
