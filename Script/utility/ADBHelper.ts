namespace TwinStar.ADBHelper {

	// ------------------------------------------------

	// TODO : move
	export function parse_output_by_line(
		source: string,
	): Array<string> {
		let destination = source.split(/[\n\r]/);
		if (destination.length > 0 && destination[destination.length - 1].length === 0) {
			destination.pop();
		}
		return destination;
	}

	// ------------------------------------------------

	export let g_run_shell_as_su: boolean = true;

	export function execute(
		argument: Array<string>,
	): string {
		let adb_path = EnvironmentVariable.search_from_path(`adb`);
		assert_test(adb_path !== null, `can not found adb path from PATH environment`);
		let buffer: Core.ByteArray;
		let buffer_content: string;
		let temporary_directory = HomeDirectory.new_temporary();
		CoreX.FileSystem.create_file(`${temporary_directory}/adb_output`);
		CoreX.FileSystem.create_file(`${temporary_directory}/adb_error`);
		let environment = CoreX.Process.environment();
		let code = CoreX.Process.execute(adb_path, argument, environment, null, `${temporary_directory}/adb_output`, `${temporary_directory}/adb_error`);
		if (code !== 0n) {
			buffer = CoreX.FileSystem.read_file(`${temporary_directory}/adb_error`);
			buffer_content = Core.Miscellaneous.cast_CharacterListView_to_JS_String(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(buffer.view())).replaceAll('\r\n', '\n');
			throw new Error(`adb execute failed : ${code}\n${buffer_content}`);
		} else {
			buffer = CoreX.FileSystem.read_file(`${temporary_directory}/adb_output`);
			buffer_content = Core.Miscellaneous.cast_CharacterListView_to_JS_String(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(buffer.view())).replaceAll('\r\n', '\n');
			return buffer_content;
		}
	}

	export function shell(
		command: string,
	): string {
		return execute([`shell`, `${g_run_shell_as_su ? 'su -c ' : ''}${command}`]);
	}

	export function pull(
		destination: string,
		source: string | Array<string>,
	): void {
		if (typeof source === 'string') {
			CoreX.FileSystem.remove(destination);
			execute([`pull`, source, destination]);
		} else {
			if (!CoreX.FileSystem.exist_directory(destination)) {
				CoreX.FileSystem.remove(destination);
				CoreX.FileSystem.create_directory(destination);
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
		temporary: string = `/sdcard/Download/TwinStar.ToolKit/ADBHelper.Temporary`,
	): void {
		remove(temporary);
		create_directory(temporary);
		if (typeof source === 'string') {
			remove(destination);
			execute([`push`, source, `${temporary}`]);
			copy(`${temporary}/${PathUtility.name(source)}`, destination);
		} else {
			if (!exist_directory(destination)) {
				remove(destination);
				create_directory(destination);
			}
			execute([`push`, ...source, `${temporary}`]);
			for (let element of source) {
				if (exist(`${destination}/${PathUtility.name(element)}`)) {
					remove(`${destination}/${PathUtility.name(element)}`);
				}
				copy(`${temporary}/${PathUtility.name(element)}`, `${destination}/${PathUtility.name(element)}`);
			}
		}
		change_fuse_owner_group(destination, application.user);
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
		let match: RegExpMatchArray | null;
		let result: ApplicationInformation = {} as any;
		result.id = id;
		shell_result = shell(`pm dump ${id}`);
		match = /versionCode=([0-9]+)/.exec(shell_result);
		assert_test(match !== null);
		result.version_code = BigInt(match[1]);
		match = /versionName=([0-9a-zA-Z\.\+\-]+)/.exec(shell_result);
		assert_test(match !== null);
		result.version_name = match[1];
		match = /userId=([0-9]+)/.exec(shell_result);
		assert_test(match !== null);
		let userIDNumber = BigInt(match[1]);
		result.user = `u${userIDNumber / 100000n}_a${userIDNumber - 10000n}`;
		return result;
	}

	export function find_application(
		rule: RegExp,
	): Array<string> {
		let shell_result: string;
		shell_result = shell(`pm list packages`);
		let id = parse_output_by_line(shell_result).map((e) => (e.substring(8))).filter((e) => (rule.test(e)));
		return id;
	}

	// ------------------------------------------------

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
		return parse_output_by_line(shell_result)[0] === 'y';
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
		return parse_output_by_line(shell_result)[0] === 'y';
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
		return parse_output_by_line(shell_result)[0] === 'y';
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