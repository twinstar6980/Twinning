/** 对外壳回调的包装 */
namespace TwinKleS.Shell {

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

	/** 外壳名 */
	export const name = callback(['name'])[0];

	/**
	 * 输入文本
	 * @param message 提示信息
	 * @returns 所输入的文本
	 * @description input $message -> $text
	 */
	export function input(
		message: string,
	): string {
		let result = callback(['input', message]);
		return result[0];
	}

	/**
	 * 输出文本
	 * @param text 需输出的文本
	 * @returns 无
	 * @description output $text -> 
	 */
	export function output(
		text: string,
	): void {
		let result = callback(['output', text]);
		return;
	}

	/**
	 * 暂停，等待用户响应
	 * @param message 提示信息
	 * @returns 无
	 * @description pause $message -> 
	 */
	export function pause(
		message: string,
	): void {
		let result = callback(['pause', message]);
		return;
	}

	export namespace Windows {

		/**
		 * 弹出文件选择对话框，要求用户选择任意文件或目录
		 * @param pick_folder 是否选择目录
		 * @param multiple 是否允许多选
		 * @returns 选中的文件路径
		 * @description select_file_by_dialog $pick_folder $multiple -> $path
		 */
		export function select_file_by_dialog(
			pick_folder: boolean,
			multiple: boolean,
		): Array<string> {
			let result = callback(['select_file_by_dialog', `${pick_folder}`, `${multiple}`]);
			return result;
		}

		export namespace CLI {

			/** 标准句柄名称 */
			type StandardHandleName = 'input' | 'output' | 'error';

			/**
			 * 获取当前控制台模式
			 * @param handle_name 句柄名称
			 * @returns 模式
			 * @description get_standard_console_mode $handle_name -> $mode
			 */
			export function get_standard_console_mode(
				handle_name: StandardHandleName,
			): number {
				let result = callback(['get_standard_console_mode', handle_name]);
				return Number.parseInt(result[0]);
			}

			/**
			 * 设置当前控制台模式
			 * @param handle_name 句柄名称
			 * @param mode 模式
			 * @returns 无
			 * @description set_standard_console_mode $handle_name $mode -> 
			 */
			export function set_standard_console_mode(
				handle_name: StandardHandleName,
				mode: number,
			): void {
				let result = callback(['set_standard_console_mode', handle_name, `${mode}`]);
				return;
			}

			/** 输入控制台模式 */
			export enum InputConsoleMode {
				ENABLE_ECHO_INPUT = 0x0004,
				ENABLE_INSERT_MODE = 0x0020,
				ENABLE_LINE_INPUT = 0x0002,
				ENABLE_MOUSE_INPUT = 0x0010,
				ENABLE_PROCESSED_INPUT = 0x0001,
				ENABLE_QUICK_EDIT_MODE = 0x0040,
				ENABLE_WINDOW_INPUT = 0x0008,
				ENABLE_VIRTUAL_TERMINAL_INPUT = 0x0200,
			};

			/** 输出控制台模式 */
			export enum OutputConsoleMode {
				ENABLE_PROCESSED_OUTPUT = 0x0001,
				ENABLE_WRAP_AT_EOL_OUTPUT = 0x0002,
				ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004,
				DISABLE_NEWLINE_AUTO_RETURN = 0x0008,
				ENABLE_LVB_GRID_WORLDWIDE = 0x0010,
			};

		}

	}

}