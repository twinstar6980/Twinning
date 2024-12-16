namespace Twinning.Script.Console {

	// ------------------------------------------------

	/**
	 * 消息分类
	 * + verbosity   常规
	 * + information 信息
	 * + warning     警告
	 * + error       错误
	 * + success     成功
	 * + input       输入
	 */
	export type MessageType = 'verbosity' | 'information' | 'warning' | 'error' | 'success' | 'input';

	// ------------------------------------------------

	export let g_basic_disable_virtual_terminal_sequence = false;

	const k_basic_message_text_attribute: Record<MessageType, VirtualTerminalSequence.TextAttribute> = {
		verbosity: {
			background: null,
			foreground: 'default',
			underline: false,
			bold: false,
		},
		information: {
			background: null,
			foreground: ['blue', false],
			underline: null,
			bold: false,
		},
		warning: {
			background: null,
			foreground: ['yellow', false],
			underline: null,
			bold: false,
		},
		error: {
			background: null,
			foreground: ['red', false],
			underline: null,
			bold: false,
		},
		success: {
			background: null,
			foreground: ['green', false],
			underline: null,
			bold: false,
		},
		input: {
			background: null,
			foreground: ['magenta', false],
			underline: false,
			bold: true,
		},
	};

	function basic_set_message_text_attribute(
		type: MessageType,
	): void {
		if (!g_basic_disable_virtual_terminal_sequence) {
			Shell.basic_output_text(VirtualTerminalSequence.text_attribute(k_basic_message_text_attribute[type]));
		}
		return;
	}

	// ------------------------------------------------

	function common_input_with_checker<Value>(
		inputer: () => string,
		echoer: (value: string) => void,
		converter: (value: string) => string | [null | Value],
		nullable: boolean,
		checker: Check.CheckerX<Value>,
		initial: undefined | null | Value,
	): null | Value {
		let result: null | Value;
		while (true) {
			let state: null | string = null;
			let input: null | string = null;
			result = null;
			if (initial !== undefined) {
				result = initial;
				initial = undefined;
			}
			else {
				input = inputer();
				echoer(input);
				let convert_result = converter(input);
				if (is_string(convert_result)) {
					state = convert_result;
				}
				else {
					result = convert_result[0];
				}
			}
			if (state === null) {
				if (result === null) {
					state = nullable ? null : los('console:not_nullable');
				}
				else {
					state = checker(result);
				}
			}
			if (state === null) {
				break;
			}
			warning(los('console:input_invalid_should_retry'), [state]);
		}
		return result;
	}

	// ------------------------------------------------

	function basic_common_output(
		text: string,
		leading: boolean,
		indent: number,
		line_feed: boolean,
	): void {
		Shell.basic_output_text(`${leading ? '●' : ' '} ${'  '.repeat(indent)}${text}${line_feed ? '\n' : ''}`);
		return;
	}

	function basic_common_input<Value>(
		leading: string,
		messager: () => void,
		converter: (value: string) => string | [null | Value],
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial: undefined | null | Value,
	): null | Value {
		let first_input = true;
		return common_input_with_checker(
			() => {
				if (first_input) {
					first_input = false;
					messager();
				}
				basic_set_message_text_attribute('input');
				basic_common_output(leading, true, 0, true);
				basic_set_message_text_attribute('verbosity');
				basic_common_output('', false, 1, false);
				return Shell.basic_input_text().text;
			},
			(value) => {
				return;
			},
			converter,
			not_null_or(nullable, false),
			not_null_or(checker, () => (null)),
			initial,
		);
	}

	// ------------------------------------------------

	function assistant_common_output(
		type: MessageType,
		title: string,
		description: Array<string>,
	): void {
		Shell.assistant_send_message(type, title, description);
		return;
	}

	function assistant_common_input<Value>(
		inputer: () => string,
		leading: string,
		echoer: (value: string) => string,
		converter: (value: string) => string | [null | Value],
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial: undefined | null | Value,
	): null | Value {
		return common_input_with_checker(
			inputer,
			(value) => {
				assistant_common_output('input', leading, [echoer(value)]);
				return;
			},
			converter,
			not_null_or(nullable, false),
			not_null_or(checker, () => (null)),
			initial,
		);
	}

	// ------------------------------------------------

	export function message(
		type: MessageType,
		title: string,
		description: Array<string>,
	): void {
		if (Shell.is_basic) {
			basic_set_message_text_attribute(type);
			basic_common_output(title, true, 0, true);
			basic_set_message_text_attribute('verbosity');
			for (let description_item of description) {
				basic_common_output(description_item, false, 1, true);
			}
		}
		if (Shell.is_assistant) {
			assistant_common_output(type, title, description);
		}
		return;
	}

	export function verbosity(
		title: string,
		description: Array<string>,
	): void {
		return message('verbosity', title, description);
	}

	export function information(
		title: string,
		description: Array<string>,
	): void {
		return message('information', title, description);
	}

	export function warning(
		title: string,
		description: Array<string>,
	): void {
		return message('warning', title, description);
	}

	export function error(
		title: string,
		description: Array<string>,
	): void {
		return message('error', title, description);
	}

	export function success(
		title: string,
		description: Array<string>,
	): void {
		return message('success', title, description);
	}

	export function error_of(
		detail: any,
	): void {
		let [title, description] = parse_error_message(detail);
		error(title, description);
		return;
	}

	// ------------------------------------------------

	export function pause(
	): void {
		let leading = 'Pause';
		if (Shell.is_basic) {
			basic_set_message_text_attribute('input');
			basic_common_output(leading + ' ', true, 0, false);
			basic_set_message_text_attribute('verbosity');
			if (KernelX.is_windows) {
				KernelX.Process.execute_system_command(`pause > NUL`);
				Shell.basic_output_text('\n');
			}
			if (KernelX.is_linux || KernelX.is_macintosh || KernelX.is_android || KernelX.is_iphone) {
				Shell.basic_input_text();
			}
		}
		if (Shell.is_assistant) {
			Shell.assistant_receive_submission('pause', []);
			assistant_common_output('input', leading, []);
		}
		return;
	}

	// ------------------------------------------------

	export function boolean(
		nullable: null,
		checker: null | Check.CheckerX<boolean>,
		initial?: boolean,
	): boolean;

	export function boolean(
		nullable: boolean,
		checker: null | Check.CheckerX<boolean>,
		initial?: null | boolean,
	): null | boolean;

	export function boolean(
		nullable: null | boolean,
		checker: null | Check.CheckerX<boolean>,
		initial?: null | boolean,
	): null | boolean {
		let result: null | boolean = undefined!;
		let leading = 'Boolean';
		let converter = (value: string): string | [null | boolean] => {
			if (value === '') {
				return [null];
			}
			let regexp_check_result = Check.enumeration_checker_x(['n', 'y'])(value);
			if (regexp_check_result !== null) {
				return los('console:boolean_format_error');
			}
			return [parse_confirmation_boolean_string(value)];
		};
		if (Shell.is_basic) {
			result = basic_common_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_assistant) {
			result = assistant_common_input(
				() => {
					return Shell.assistant_receive_submission('boolean', []).value;
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function integer(
		nullable: null,
		checker: null | Check.CheckerX<bigint>,
		initial?: bigint,
	): bigint;

	export function integer(
		nullable: boolean,
		checker: null | Check.CheckerX<bigint>,
		initial?: null | bigint,
	): null | bigint;

	export function integer(
		nullable: null | boolean,
		checker: null | Check.CheckerX<bigint>,
		initial?: null | bigint,
	): null | bigint {
		let result: null | bigint = undefined!;
		let leading = 'Integer';
		let converter = (value: string): string | [null | bigint] => {
			if (value === '') {
				return [null];
			}
			let regexp_check_result = Check.regexp_checker_x(/^([+-])?([\d]+)$/)(value);
			if (regexp_check_result !== null) {
				return los('console:integer_format_error', regexp_check_result);
			}
			return [BigInt(value)];
		};
		if (Shell.is_basic) {
			result = basic_common_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_assistant) {
			result = assistant_common_input(
				() => {
					return Shell.assistant_receive_submission('integer', []).value;
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function floater(
		nullable: null,
		checker: null | Check.CheckerX<number>,
		initial?: number,
	): number;

	export function floater(
		nullable: boolean,
		checker: null | Check.CheckerX<number>,
		initial?: null | number,
	): null | number;

	export function floater(
		nullable: null | boolean,
		checker: null | Check.CheckerX<number>,
		initial?: null | number,
	): null | number {
		let result: null | number = undefined!;
		let leading = 'Floater';
		let converter = (value: string): string | [null | number] => {
			if (value === '') {
				return [null];
			}
			let regexp_check_result = Check.regexp_checker_x(/^([+-])?([\d]+)([.][\d]+)?$/)(value);
			if (regexp_check_result !== null) {
				return los('console:floater_format_error', regexp_check_result);
			}
			return [Number(value)];
		};
		if (Shell.is_basic) {
			result = basic_common_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_assistant) {
			result = assistant_common_input(
				() => {
					return Shell.assistant_receive_submission('floater', []).value;
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function size(
		nullable: null,
		checker: null | Check.CheckerX<bigint>,
		initial?: bigint,
	): bigint;

	export function size(
		nullable: boolean,
		checker: null | Check.CheckerX<bigint>,
		initial?: null | bigint,
	): null | bigint;

	export function size(
		nullable: null | boolean,
		checker: null | Check.CheckerX<bigint>,
		initial?: null | bigint,
	): null | bigint {
		let result: null | bigint = undefined!;
		let leading = 'Size';
		let converter = (value: string): string | [null | bigint] => {
			if (value === '') {
				return [null];
			}
			let regexp_check_result = Check.regexp_checker_x(/^([\d]+)([.][\d]+)?([bkmg])$/)(value);
			if (regexp_check_result !== null) {
				return los('console:size_format_error', regexp_check_result);
			}
			return [parse_size_string(value)];
		};
		if (Shell.is_basic) {
			result = basic_common_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_assistant) {
			result = assistant_common_input(
				() => {
					return Shell.assistant_receive_submission('size', []).value;
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function string(
		nullable: null,
		checker: null | Check.CheckerX<string>,
		initial?: string,
	): string;

	export function string(
		nullable: boolean,
		checker: null | Check.CheckerX<string>,
		initial?: null | string,
	): null | string;

	export function string(
		nullable: null | boolean,
		checker: null | Check.CheckerX<string>,
		initial?: null | string,
	): null | string {
		let result: null | string = undefined!;
		let leading = 'String';
		let converter = (value: string): string | [null | string] => {
			if (value === '') {
				return [null];
			}
			let result: string;
			if (value[0] !== ':') {
				result = value;
			}
			else {
				switch (value[1]) {
					case ':': {
						result = value.slice(2);
						break;
					}
					default: {
						return los('console:input_command_invalid');
					}
				}
			}
			return [result];
		};
		if (Shell.is_basic) {
			result = basic_common_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_assistant) {
			result = assistant_common_input(
				() => {
					return Shell.assistant_receive_submission('string', []).value;
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function path(
		type: 'any' | 'file' | 'directory',
		mode: 'any' | 'input' | 'output',
		nullable: null,
		checker: null | Check.CheckerX<string>,
		initial?: string,
	): string;

	export function path(
		type: 'any' | 'file' | 'directory',
		mode: 'any' | 'input' | 'output',
		nullable: boolean,
		checker: null | Check.CheckerX<string>,
		initial?: null | string,
	): null | string;

	export function path(
		type: 'any' | 'file' | 'directory',
		mode: 'any' | 'input' | 'output',
		nullable: null | boolean,
		checker: null | Check.CheckerX<string>,
		initial?: null | string,
	): null | string {
		let result: null | string = undefined!;
		let leading = 'Path';
		let state_data = {
			last_value: null as null | string,
			allow_overwrite: false as boolean,
		};
		if (initial !== undefined && initial !== null) {
			initial = Home.of(PathUtility.regularize(initial));
			state_data.last_value = initial;
		}
		let converter = (value: string): string | [null | string] => {
			if (value === '') {
				return [null];
			}
			let result: string;
			if (value[0] !== ':') {
				result = Home.of(PathUtility.regularize(unquote_string(value)));
			}
			else {
				switch (value[1]) {
					case ':': {
						result = Home.of(PathUtility.regularize(value.slice(2)));
						break;
					}
					case 'p': {
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						let pick_type = null as null | 'load_file' | 'load_directory' | 'save_file';
						if (mode === 'input' && type === 'file') {
							pick_type = 'load_file';
						}
						if (mode === 'input' && type === 'directory') {
							pick_type = 'load_directory';
						}
						if (mode === 'output' && type === 'file') {
							pick_type = 'save_file';
						}
						let pick_result = pick_storage_item(pick_type);
						if (pick_result === null) {
							return los('console:path_command_pick_cancel');
						}
						result = pick_result;
						message('input', leading, [result]);
						break;
					}
					case 'g': {
						if (mode !== 'output') {
							return los('console:path_command_need_output');
						}
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						if (state_data.last_value === null) {
							return los('console:input_command_need_previous_input');
						}
						result = state_data.last_value;
						if (KernelX.Storage.exist(result)) {
							result = PathUtility.generate_suffix_path(result);
							warning(los('console:path_is_exist_but_generate'), [result]);
						}
						break;
					}
					case 'm': {
						if (mode !== 'output') {
							return los('console:path_command_need_output');
						}
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						if (state_data.last_value === null) {
							return los('console:input_command_need_previous_input');
						}
						result = state_data.last_value;
						if (KernelX.Storage.exist(result)) {
							let move_path = PathUtility.generate_suffix_path(result);
							KernelX.Storage.rename(result, move_path);
							warning(los('console:path_is_exist_but_move'), [move_path]);
						}
						break;
					}
					case 'd': {
						if (mode !== 'output') {
							return los('console:path_command_need_output');
						}
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						if (state_data.last_value === null) {
							return los('console:input_command_need_previous_input');
						}
						result = state_data.last_value;
						if (KernelX.Storage.exist(result)) {
							KernelX.Storage.remove(result);
							warning(los('console:path_is_exist_but_delete'), []);
						}
						break;
					}
					case 'o': {
						if (mode !== 'output') {
							return los('console:path_command_need_output');
						}
						if (value.length > 2) {
							return los('console:input_command_invalid');
						}
						if (state_data.last_value === null) {
							return los('console:input_command_need_previous_input');
						}
						result = state_data.last_value;
						if (KernelX.Storage.exist(result)) {
							state_data.allow_overwrite = true;
							warning(los('console:path_is_exist_but_overwrite'), []);
						}
						break;
					}
					default: {
						return los('console:input_command_invalid');
					}
				}
			}
			return [result];
		};
		let checker_proxy = (value: string): null | string => {
			if (value.length === 0) {
				return los('console:path_is_empty');
			}
			if (mode === 'input') {
				if (!KernelX.Storage.exist(value)) {
					return los('console:path_not_exist');
				}
				if (type === 'file' && !KernelX.Storage.exist_file(value)) {
					return los('console:path_is_exist_not_file');
				}
				if (type === 'directory' && !KernelX.Storage.exist_directory(value)) {
					return los('console:path_is_exist_not_directory');
				}
			}
			if (mode === 'output') {
				if (!state_data.allow_overwrite && KernelX.Storage.exist(value)) {
					return los('console:path_is_exist');
				}
			}
			state_data.last_value = value;
			state_data.allow_overwrite = false;
			return checker === null ? null : checker(value);
		};
		if (Shell.is_basic) {
			result = basic_common_input(
				leading,
				() => {
					return;
				},
				converter,
				nullable,
				checker_proxy,
				initial,
			);
		}
		if (Shell.is_assistant) {
			result = assistant_common_input(
				() => {
					return Shell.assistant_receive_submission('path', []).value;
				},
				leading,
				(value) => {
					return value;
				},
				converter,
				nullable,
				checker_proxy,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function enumeration<Value>(
		option: Array<[Value, string, null | string]>,
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial?: Value,
	): Value;

	export function enumeration<Value>(
		option: Array<[Value, string, null | string]>,
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial?: null | Value,
	): null | Value;

	export function enumeration<Value>(
		option: Array<[Value, string, null | string]>,
		nullable: null | boolean,
		checker: null | Check.CheckerX<Value>,
		initial?: null | Value,
	): null | Value {
		let result: null | Value = undefined!;
		let leading = 'Enumeration';
		let maximum_key_length = Math.max(...option.map((value) => (value[1].length)));
		let message = option.map((value) => (`${make_prefix_padded_string(value[1], ' ', maximum_key_length)}${value[2] === null ? '' : `. ${value[2]}`}`));
		let converter = (value: string): string | [null | Value] => {
			if (value === '') {
				return [null];
			}
			let index = option.findIndex((e) => (e[1] === value));
			if (index === -1) {
				return los('console:option_invalid');
			}
			return [option[index][0]];
		};
		if (Shell.is_basic) {
			result = basic_common_input(
				leading,
				() => {
					for (let messageItem of message) {
						basic_common_output(messageItem, false, 1, true);
					}
					return;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		if (Shell.is_assistant) {
			result = assistant_common_input(
				() => {
					let value = Shell.assistant_receive_submission('enumeration', message).value;
					return value === '' ? '' : option[message.indexOf(value)][1];
				},
				leading,
				(value) => {
					if (value === '') {
						return '';
					}
					let index = option.findIndex((e) => (e[1] === value));
					if (index === -1) {
						return los('console:option_invalid');
					}
					return message[index];
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ------------------------------------------------

	export function option_boolean<Value extends boolean>(
		value: Array<Value>,
	): Array<[Value, string, null | string]> {
		return value.map((value, index) => ([value, `${value === false ? 'n' : 'y'}`, null]));
	}

	export function option_floater<Value extends number>(
		value: Array<Value>,
	): Array<[Value, string, null | string]> {
		return value.map((value, index) => ([value, `${value}`, null]));
	}

	export function option_integer<Value extends bigint>(
		value: Array<Value>,
	): Array<[Value, string, null | string]> {
		return value.map((value, index) => ([value, `${value}`, null]));
	}

	export function option_string<Value extends string>(
		value: Array<Value>,
	): Array<[Value, string, null | string]> {
		return value.map((value, index) => ([value, `${index + 1}`, `${value}`]));
	}

	// ------------------------------------------------

	export function pick_storage_item(
		type: null | 'load_file' | 'load_directory' | 'save_file',
	): null | string {
		let result: null | string = undefined!;
		if (type === null) {
			information(los('console:pick_path_type'), []);
			type = enumeration(option_string(['load_file', 'load_directory', 'save_file']), null, null) as 'load_file' | 'load_directory' | 'save_file';
		}
		if (Shell.is_basic) {
			result = Shell.basic_pick_storage_item(type).target;
		}
		if (Shell.is_assistant) {
			result = Shell.assistant_pick_storage_item(type).target;
		}
		if (result === '') {
			result = null;
		}
		return result;
	}

	// ------------------------------------------------

	export function push_system_notification(
		title: string,
		description: string,
	): void {
		if (Shell.is_basic) {
			Shell.basic_push_system_notification(title, description);
		}
		if (Shell.is_assistant) {
			Shell.assistant_push_system_notification(title, description);
		}
		return;
	}

	// ------------------------------------------------

}