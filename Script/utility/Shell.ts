namespace TwinStar.Script.Shell {

	// ------------------------------------------------

	export function callback(
		argument: Array<string>,
	): Array<string> {
		return CoreX.Miscellaneous.callback(argument);
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

	// ------------------------------------------------

	export function gui_output_message(
		type: 'v' | 'i' | 'w' | 'e' | 's' | 't',
		title: string,
		description: Array<string>,
	): void {
		let result = callback(['output_message', type, title, ...description]);
		return;
	}

	export function gui_input_pause(
	): void {
		let result = callback(['input_pause']);
		return;
	}

	export function gui_input_confirmation(
	): string | null {
		let result = callback(['input_confirmation']);
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

	export function gui_input_path(
	): string | null {
		let result = callback(['input_path']);
		return result[0].length === 0 ? null : result[0];
	}

	export function gui_input_option(
		option: Array<string>,
	): string | null {
		let result = callback(['input_option', ...option]);
		return result[0].length === 0 ? null : result[0];
	}

	export function gui_push_notification(
		title: string,
		description: string,
	): void {
		let result = callback(['push_notification', title, description]);
		return;
	}

	// ------------------------------------------------

}