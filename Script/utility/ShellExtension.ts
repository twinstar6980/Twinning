namespace TwinStar.Script.ShellExtension {

	// ------------------------------------------------

	export let g_program_path: null | string = null;

	// ------------------------------------------------

	export function execute(
		argument: Array<string>,
		program: null | string = g_program_path,
	): Array<string> {
		if (program === null) {
			throw new Error(`shell extension program is not available`);
		}
		let result = ProcessHelper.execute(program, argument, CoreX.Process.list_environment_variable());
		if (result.code !== 0n) {
			throw new Error(`shell extension invoke failed : ${result.code} (${result.error})`);
		}
		return split_string_by_line_feed(result.error).slice(1);
	}

	export function test(
		program: string,
	): boolean {
		try {
			execute(['print_help'], program);
			return true;
		} catch (e) {
			return false;
		}
	}

	// ------------------------------------------------

	export function push_notification_avaliable(
	): boolean {
		return g_program_path !== null && (Shell.is_windows || Shell.is_linux || Shell.is_macintosh);
	}

	export function push_notification(
		title: string,
		description: string,
	): void {
		let result = execute(['push_notification', title, description]);
		return;
	}

	export function select_path_avaliable(
	): boolean {
		return g_program_path !== null && (Shell.is_windows || Shell.is_linux || Shell.is_macintosh);
	}

	export function select_path(
		is_directory: boolean,
		is_multiple: boolean,
		initial_path: string,
	): Array<string> {
		let result = execute(['select_path', `${is_directory}`, `${is_multiple}`, initial_path]);
		return result;
	}

	// ------------------------------------------------

}