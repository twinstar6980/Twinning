namespace TwinStar.Script.Shell {

	// ------------------------------------------------

	export function callback(
		argument: Array<string>,
	): Array<string> {
		return KernelX.Miscellaneous.callback(argument);
	}

	// ------------------------------------------------

	export const host = String(callback(['host'])[0]);

	export const system = String(callback(['system'])[0]);

	// ------------------------------------------------

	export const is_cli = host === 'cli';

	export const is_gui = host === 'gui' || host === 'helper'; // NOTE : helper is superset of gui

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

	export function cli_pick_path(
		type: 'file' | 'directory',
	): string {
		let result = callback(['pick_path', type]);
		return result[0];
	}

	export function cli_push_notification(
		title: string,
		description: string,
	): void {
		let result = callback(['push_notification', title, description]);
		return;
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

	export function gui_input_boolean(
	): string {
		let result = callback(['input_boolean']);
		return result[0];
	}

	export function gui_input_integer(
	): string {
		let result = callback(['input_integer']);
		return result[0];
	}

	export function gui_input_floater(
	): string {
		let result = callback(['input_floater']);
		return result[0];
	}

	export function gui_input_size(
	): string {
		let result = callback(['input_size']);
		return result[0];
	}

	export function gui_input_string(
	): string {
		let result = callback(['input_string']);
		return result[0];
	}

	export function gui_input_path(
	): string {
		let result = callback(['input_path']);
		return result[0];
	}

	export function gui_input_enumeration(
		option: Array<string>,
	): string {
		let result = callback(['input_enumeration', ...option]);
		return result[0];
	}

	export function gui_pick_path(
		type: 'file' | 'directory',
	): string {
		let result = callback(['pick_path', type]);
		return result[0];
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