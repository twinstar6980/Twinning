/** 对外壳回调的包装 */
namespace TwinKleS.Shell {

	// ------------------------------------------------

	/** 调用外壳回调，并判断是否出现异常 */
	export function callback(
		argument: Array<string>,
	): Array<string> {
		let result = CoreX.Misc.shell_callback(argument);
		if (result[0].length !== 0) {
			throw new MyError(`shell callback invoke failed : ${result[0]}`);
		}
		return result.slice(1);
	}

	// ------------------------------------------------

	/** 外壳名 */
	export const name = callback(['name'])[0] as 'windows.cli' | 'linux.cli' | 'macos.cli' | 'android.cli' | 'ios.cli';

	export const is_cli = name.endsWith('.cli');

	export const is_gui = name.endsWith('.gui');

	export const is_windows = name.startsWith('windows.');

	export const is_linux = name.startsWith('linux.');

	export const is_macos = name.startsWith('macos.');

	export const is_android = name.startsWith('android.');

	export const is_ios = name.startsWith('ios.');

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

	/**
	 * 弹出文件选择对话框，要求用户选择任意文件或目录
	 * @param pick_folder 是否选择目录
	 * @param multiple 是否允许多选
	 * @returns 选中的文件路径
	 * @description select_file_by_dialog $pick_folder $multiple -> $path
	 */
	export function windows_cli_open_file_dialog(
		pick_folder: boolean,
		multiple: boolean,
	): Array<string> {
		let result = callback(['open_file_dialog', `${pick_folder}`, `${multiple}`]);
		return result;
	}

	// ------------------------------------------------

	export function gui_output(
		type: 'v' | 'i' | 'w' | 'e' | 's' | 't',
		message: string,
		sub_message: Array<string>,
	): void {
		let result = callback(['output', type, message, ...sub_message]);
		return;
	}

	export function gui_input_string(
	): string {
		let result = callback(['input_string']);
		return result[0];
	}

	export function gui_input_option(
		option: Array<string>,
	): bigint {
		let result = callback(['input_option', ...option]);
		return BigInt(result[0]);
	}

	// ------------------------------------------------

}