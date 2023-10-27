namespace TwinStar.Script.ADBHelper {

	// ------------------------------------------------

	const k_remote_temporary_directory = `/data/local/tmp/TwinStar.ToolKit.ADBHelper.Temporary`;

	// ------------------------------------------------

	export let g_shell_run_as_su: boolean = true;

	export function execute(
		argument: Array<string>,
	): string {
		let adb_path = ProcessHelper.search_path(`adb`);
		assert_test(adb_path !== null, `can not found 'adb' program from PATH environment`);
		let execute_result = ProcessHelper.execute(adb_path, argument, KernelX.Process.list_environment_variable());
		if (execute_result.code !== 0n) {
			throw new Error(`adb execute failed : ${execute_result.code}\n${execute_result.error}`);
		} else {
			return execute_result.output;
		}
	}

	export function shell(
		command: string,
	): string {
		return execute([`shell`, `${g_shell_run_as_su ? 'su -c ' : ''}${command}`]);
	}

	export function pull(
		destination: string,
		source: string | Array<string>,
	): void {
		if (typeof source === 'string') {
			KernelX.FileSystem.remove(destination);
			execute([`pull`, source, destination]);
		} else {
			if (!KernelX.FileSystem.exist_directory(destination)) {
				KernelX.FileSystem.remove(destination);
				KernelX.FileSystem.create_directory(destination);
			}
			execute([`pull`, ...source, destination]);
		}
		return;
	}

	export function push(
		destination: string,
		source: string | Array<string>,
	): void {
		if (typeof source === 'string') {
			remove(destination);
			execute([`push`, source, destination]);
		} else {
			if (!exist_directory(destination)) {
				remove(destination);
				create_directory(destination);
			}
			execute([`push`, ...source, destination]);
		}
		return;
	}

	export function push_secure(
		destination: string,
		source: string | Array<string>,
		application: ApplicationInformation,
	): void {
		remove(k_remote_temporary_directory);
		create_directory(k_remote_temporary_directory);
		change_mode(k_remote_temporary_directory, `777`);
		if (typeof source === 'string') {
			remove(destination);
			execute([`push`, source, `${k_remote_temporary_directory}`]);
			copy(`${k_remote_temporary_directory}/${PathUtility.name(source)}`, destination);
		} else {
			if (!exist_directory(destination)) {
				remove(destination);
				create_directory(destination);
			}
			execute([`push`, ...source, `${k_remote_temporary_directory}`]);
			for (let element of source) {
				if (exist(`${destination}/${PathUtility.name(element)}`)) {
					remove(`${destination}/${PathUtility.name(element)}`);
				}
				copy(`${k_remote_temporary_directory}/${PathUtility.name(element)}`, `${destination}/${PathUtility.name(element)}`);
			}
		}
		change_fuse_owner_group(destination, application.user);
		remove(k_remote_temporary_directory);
		return;
	}

	// ------------------------------------------------

	export type ApplicationInformation = {
		id: string;
		user: string;
		version_code: bigint;
		version_name: string;
	};

	export function get_application_information(
		id: string,
	): ApplicationInformation {
		let shell_result: string;
		let match: null | RegExpMatchArray;
		let result: ApplicationInformation = {} as any;
		result.id = id;
		shell_result = shell(`pm dump ${id}`);
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

	export function find_application(
		rule: RegExp,
	): Array<string> {
		let shell_result: string;
		shell_result = shell(`pm list packages`);
		let id = split_string_by_line_feed(shell_result).map((e) => (e.substring(8))).filter((e) => (rule.test(e)));
		return id;
	}

	// ------------------------------------------------

	export function change_mode(
		target: string,
		mode: string,
	): void {
		let shell_result: string;
		shell_result = ADBHelper.shell(`chmod -R ${mode} "${target}"`);
		return;
	}

	export function change_owner(
		target: string,
		owner: string,
	): void {
		let shell_result: string;
		shell_result = ADBHelper.shell(`chown -R ${owner} "${target}"`);
		return;
	}

	export function change_group(
		target: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = ADBHelper.shell(`chgrp -R ${group} "${target}"`);
		return;
	}

	export function change_owner_group(
		target: string,
		owner: string,
		group: string,
	): void {
		let shell_result: string;
		shell_result = ADBHelper.shell(`chown -R ${owner}:${group} "${target}"`);
		return;
	}

	export function change_fuse_owner_group(
		target: string,
		owner: string,
	): void {
		if (PathUtility.is_android_fuse_object(target)) {
			if (PathUtility.is_android_fuse_ext_data_object(target)) {
				change_owner_group(target, owner, 'ext_data_rw');
			} else if (PathUtility.is_android_fuse_ext_obb_object(target)) {
				change_owner_group(target, owner, 'ext_obb_rw');
			} else {
				change_owner_group(target, owner, 'media_rw');
			}
		}
		return;
	}

	// ------------------------------------------------

	export function exist(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = ADBHelper.shell(`
			if [ -e "${target}" ];then
				echo "y"
			else
				echo "n"
			fi
		`);
		return split_string_by_line_feed(shell_result)[0] === 'y';
	}

	export function exist_file(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = ADBHelper.shell(`
			if [ -f "${target}" ];then
				echo "y"
			else
				echo "n"
			fi
		`);
		return split_string_by_line_feed(shell_result)[0] === 'y';
	}

	export function exist_directory(
		target: string,
	): boolean {
		let shell_result: string;
		shell_result = ADBHelper.shell(`
			if [ -d "${target}" ];then
				echo "y"
			else
				echo "n"
			fi
		`);
		return split_string_by_line_feed(shell_result)[0] === 'y';
	}

	export function copy(
		source: string,
		destination: string,
	): void {
		let shell_result: string;
		shell_result = ADBHelper.shell(`cp -rf "${source}" "${destination}"`);
		return;
	}

	export function rename(
		source: string,
		destination: string,
	): void {
		let shell_result: string;
		shell_result = ADBHelper.shell(`mv -f "${source}" "${destination}"`);
		return;
	}

	export function remove(
		target: string,
	): void {
		let shell_result: string;
		shell_result = ADBHelper.shell(`rm -rf "${target}"`);
		return;
	}

	export function create_directory(
		target: string,
	): void {
		let shell_result: string;
		shell_result = ADBHelper.shell(`mkdir -p "${target}"`);
		return;
	}

	// ------------------------------------------------

}