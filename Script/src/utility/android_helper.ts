namespace Twinning.Script.AndroidHelper {

	// #region common

	function escape(
		source: string,
	): string {
		return source.replaceAll(/(?=['" ])/g, `\\`);
	}

	// ----------------

	const k_temporary_directory = new StoragePath(`/data/local/tmp/twinning`);

	function make_temporary_directory(
	): StoragePath {
		fs_create_directory(k_temporary_directory, null);
		fs_ensure_public_access([k_temporary_directory]);
		return k_temporary_directory;
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
			result = ExternalHelper.run_sh([`-c`, `su -c "${command}"`]);
		}
		if (k_mode === 'bridge') {
			result = ExternalHelper.run_adb([`shell`, `su -c "${command}"`]);
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
			ExternalHelper.run_adb([`pull`, remote.emit_posix(true), local.emit_native()]);
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
				ExternalHelper.run_adb([`push`, local.emit_native(), remote.emit_posix(true)]);
			}
			else {
				let remote_temporary = make_temporary_directory().join(local.name() ?? '');
				ExternalHelper.run_adb([`push`, local.emit_native(), remote_temporary.emit_posix(true)]);
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
		shell_result = shell(`if [ -e ${escape(target.emit_posix(true))} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_copy(
		target: StoragePath,
		placement: StoragePath,
		follow_link: boolean,
	): void {
		let shell_result: string;
		shell_result = shell(`cp -rf ${!follow_link ? '-P' : '-L'} ${escape(target.emit_posix(true))} ${escape(placement.emit_posix(true))}`);
		return;
	}

	export function fs_rename(
		target: StoragePath,
		placement: StoragePath,
	): void {
		let shell_result: string;
		shell_result = shell(`mv -f ${escape(target.emit_posix(true))} ${escape(placement.emit_posix(true))}`);
		return;
	}

	export function fs_remove(
		target: StoragePath,
	): void {
		let shell_result: string;
		shell_result = shell(`rm -rf ${escape(target.emit_posix(true))}`);
		return;
	}

	export function fs_exist_file(
		target: StoragePath,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -f ${escape(target.emit_posix(true))} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_exist_directory(
		target: StoragePath,
	): boolean {
		let shell_result: string;
		shell_result = shell(`if [ -d ${escape(target.emit_posix(true))} ] ; then echo y ; else echo n ; fi`);
		return ConvertHelper.split_string_by_line_feed(shell_result, true)[0] === 'y';
	}

	export function fs_create_directory(
		target: StoragePath,
		mode: null | string,
	): void {
		mode = CheckHelper.not_null_or(mode, '777');
		let shell_result: string;
		shell_result = shell(`mkdir -p -m ${escape(mode)} ${escape(target.emit_posix(true))}`);
		return;
	}

	// ----------------

	export function fs_change_mode(
		target: StoragePath,
		mode: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chmod -R ${escape(mode)} ${escape(target.emit_posix(true))}`);
		return;
	}

	export function fs_change_owner(
		target: StoragePath,
		owner: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chown -R ${escape(owner)} ${escape(target.emit_posix(true))}`);
		return;
	}

	export function fs_change_group(
		target: StoragePath,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chgrp -R ${escape(group)} ${escape(target.emit_posix(true))}`);
		return;
	}

	export function fs_change_owner_group(
		target: StoragePath,
		owner: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = shell(`chown -R ${escape(owner)}:${escape(group)} ${escape(target.emit_posix(true))}`);
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
		shell_result = shell(`pm list packages -U ${escape(application)}`);
		{
			match = new RegExp(`^package:${application.replaceAll('.', '\.')} uid\:([0-9]+)$`, 'm').exec(shell_result);
			assert_test(match !== null);
			let user_number = BigInt(match[1]);
			result.user = `u${user_number / 100000n}_a${user_number - 10000n}`;
		}
		shell_result = shell(`pm dump ${escape(application)}`);
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
	): void {
		let shell_result: string;
		shell_result = shell(`am start -n ${escape(application)}/${escape(activity)}`);
		return;
	}

	// #endregion

	// #region termux

	export function termux_run_process(
		program: StoragePath,
		argument: Array<string>,
	): ReturnType<typeof ProcessHelper.run_process> {
		let shell_result: string;
		let temporary_directory_fallback = StorageHelper.temporary('directory');
		let temporary_directory = make_temporary_directory().join(temporary_directory_fallback.name()!);
		using temporary_directory_finalizer = new Finalizer(() => {
			StorageHelper.remove(temporary_directory);
			StorageHelper.remove(temporary_directory_fallback);
		});
		let script_file = temporary_directory.join('script');
		let input_file = temporary_directory.join('stdin');
		let output_file = temporary_directory.join('stdout');
		let error_file = temporary_directory.join('stderr');
		let code_file = temporary_directory.join('exit_code');
		StorageHelper.create_directory(temporary_directory);
		StorageHelper.write_file_text(script_file, `#!/usr/bin/bash\n${[program.emit_native(), ...argument].map((it) => `"${escape(it)}"`).join(' ')}\n`);
		StorageHelper.write_file_text(input_file, '');
		StorageHelper.create_file(output_file);
		StorageHelper.create_file(error_file);
		fs_ensure_public_access([
			temporary_directory,
			script_file,
			input_file,
			output_file,
			error_file,
		]);
		shell_result = shell([
			`am startservice`,
			`--user 0`,
			`-n com.termux/com.termux.app.RunCommandService`,
			`-a com.termux.RUN_COMMAND`,
			`--es com.termux.RUN_COMMAND_PATH               ${escape(script_file.emit_native())}`,
			`--es com.termux.RUN_COMMAND_WORKDIR            ${escape(temporary_directory.emit_native())}`,
			`--ez com.termux.RUN_COMMAND_BACKGROUND         ${escape(`true`)}`,
			`--es com.termux.RUN_COMMAND_SESSION_ACTION     ${escape(`0`)}`,
			`--es com.termux.RUN_COMMAND_STDIN              ${escape(input_file.emit_native())}`,
			`--es com.termux.RUN_COMMAND_RESULT_DIRECTORY   ${escape(temporary_directory.emit_native())}`,
			`--ez com.termux.RUN_COMMAND_RESULT_SINGLE_FILE ${escape(`false`)}`,
		].join(' '));
		while (!StorageHelper.exist_file(code_file)) {
			Kernel.Miscellaneous.Thread.sleep(Kernel.Size.value(200n));
		}
		fs_ensure_public_access([
			code_file,
		]);
		let read_file = (path: StoragePath): string => {
			let data = StorageHelper.read_file_text(path);
			return ConvertHelper.normalize_string_line_feed(data);
		};
		return {
			path: program,
			code: BigInt(read_file(code_file)),
			output: read_file(output_file),
			error: read_file(error_file),
		};
	}

	// #endregion

}
