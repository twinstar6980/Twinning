namespace TwinStar.Shell {

	// ------------------------------------------------

	export function callback(
		argument: Array<string>,
	): Array<string> {
		let result = CoreX.Miscellaneous.callback(argument);
		if (result[0].length !== 0) {
			throw new Error(`callback invoke failed : ${result[0]}`);
		}
		return result.slice(1);
	}

	// ------------------------------------------------

	export const name = String(callback(['name'])[0]);

	export const version = BigInt(callback(['version'])[0]);

	export const system = String(callback(['system'])[0]);

	// ------------------------------------------------

	export const is_cli = name === 'cli';

	export const is_gui = name === 'gui';

	export const is_windows = system === 'windows';

	export const is_linux = system === 'linux';

	export const is_macintosh = system === 'macintosh';

	export const is_android = system === 'android';

	export const is_iphone = system === 'iphone';

	// ------------------------------------------------

	export function cli_output(
		text: string,
	): void {
		let result = callback(['output', text]);
		return;
	}

	export function cli_input(
	): string {
		let result = callback(['input']);
		return result[0];
	}

	export function windows_cli_open_file_dialog(
		pick_folder: boolean,
		multiple: boolean,
	): Array<string> {
		let result = callback(['open_file_dialog', `${pick_folder}`, `${multiple}`]);
		return result;
	}

	// ------------------------------------------------

	export function gui_output_notify(
		type: 'v' | 'i' | 'w' | 'e' | 's' | 't',
		title: string,
		description: Array<string>,
	): void {
		let result = callback(['output_notify', type, title, ...description]);
		return;
	}

	export function gui_input_pause(
	): void {
		let result = callback(['input_pause']);
		return;
	}

	export function gui_input_confirm(
	): string | null {
		let result = callback(['input_confirm']);
		return result[0].length === 0 ? null : result[0];
	}

	export function gui_input_number(
	): string | null {
		let result = callback(['input_number']);
		return result[0].length === 0 ? null : result[0];
	}

	export function gui_input_integer(
	): string | null {
		let result = callback(['input_integer']);
		return result[0].length === 0 ? null : result[0];
	}

	export function gui_input_size(
	): string | null {
		let result = callback(['input_size']);
		return result[0].length === 0 ? null : result[0];
	}

	export function gui_input_string(
	): string | null {
		let result = callback(['input_string']);
		return result[0].length === 0 ? null : result[0];
	}

	export function gui_input_option(
		option: Array<string>,
	): string | null {
		let result = callback(['input_option', ...option]);
		return result[0].length === 0 ? null : result[0];
	}

	// ------------------------------------------------

}