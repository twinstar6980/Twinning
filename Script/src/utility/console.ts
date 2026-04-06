namespace Twinning.Script.Console {

	// #region common

	export type MessageType = 'verbosity' | 'information' | 'warning' | 'error' | 'success' | 'input';

	// ----------------

	export let g_basic_disable_virtual_terminal_sequence: boolean = false;

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

	// #endregion

	// #region output

	function basic_common_output(
		text: string,
		leading: boolean,
		indent: number,
		line_feed: boolean,
	): void {
		Shell.basic_output_text(`${leading ? '●' : ' '} ${'  '.repeat(indent)}${text}${line_feed ? '\n' : ''}`);
		return;
	}

	function assistant_common_output(
		type: MessageType,
		title: string,
		description: Array<string>,
	): void {
		Shell.assistant_send_message(type, title, description);
		return;
	}

	// ----------------

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

	// ----------------

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

	// ----------------

	export function error_of(
		exception: any,
	): void {
		let [title, description] = ConvertHelper.generate_exception_message(exception);
		error(title, description);
		return;
	}

	// #endregion

	// #region input

	function common_input<TValue>(
		reader: () => string,
		echoer: (value: string) => void,
		converter: (representation: string | [string]) => string | [TValue],
		nullable: boolean,
		checker: CheckHelper.Checker<TValue>,
		initial: undefined | null | TValue,
	): null | TValue {
		let result: null | TValue;
		let converter_proxy = (representation: string): string | [null | TValue] => {
			if (representation === '') {
				return [null];
			}
			let result: string | [TValue];
			assert_test(representation[0] === '?');
			if (representation[1] === '?') {
				let content = representation.substring(2);
				result = converter(content);
			}
			else {
				let macro = representation.substring(1);
				result = converter([macro]);
			}
			return result;
		};
		while (true) {
			let state: null | string = null;
			let input: null | string = null;
			result = null;
			if (initial !== undefined) {
				result = initial;
				initial = undefined;
			}
			else {
				input = reader();
				echoer(input);
				let convert_result = converter_proxy(input);
				if (CheckHelper.is_string(convert_result)) {
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

	function basic_common_input<TValue>(
		leading: string,
		messenger: () => void,
		converter: (representation: string | [string]) => string | [TValue],
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<TValue>,
		initial: undefined | null | TValue,
	): null | TValue {
		let first_input = true;
		return common_input(
			() => {
				if (first_input) {
					first_input = false;
					messenger();
				}
				basic_set_message_text_attribute('input');
				basic_common_output(leading, true, 0, true);
				basic_set_message_text_attribute('verbosity');
				basic_common_output('', false, 1, false);
				let value = Shell.basic_input_text().text;
				if (value !== '' && value[0] !== '?') {
					value = `??${value}`;
				}
				return value;
			},
			(value) => {
				return;
			},
			converter,
			CheckHelper.not_null_or(nullable, false),
			CheckHelper.not_null_or(checker, () => (null)),
			initial,
		);
	}

	function assistant_common_input<TValue>(
		leading: string,
		reader: () => string,
		converter: (representation: string | [string]) => string | [TValue],
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<TValue>,
		initial: undefined | null | TValue,
	): null | TValue {
		return common_input(
			reader,
			(value) => {
				assistant_common_output('input', leading, [value]);
				return;
			},
			converter,
			CheckHelper.not_null_or(nullable, false),
			CheckHelper.not_null_or(checker, () => (null)),
			initial,
		);
	}

	// ----------------

	export function pause(
	): void {
		let leading = 'Pause';
		if (Shell.is_basic) {
			basic_set_message_text_attribute('input');
			basic_common_output(leading + ' ', true, 0, false);
			basic_set_message_text_attribute('verbosity');
			if (KernelX.is_windows) {
				KernelX.Process.execute_command(`pause > NUL`);
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

	// ----------------

	export function boolean(
		nullable: null,
		checker: null | CheckHelper.Checker<boolean>,
		initial?: boolean,
	): boolean;

	export function boolean(
		nullable: boolean,
		checker: null | CheckHelper.Checker<boolean>,
		initial?: null | boolean,
	): null | boolean;

	export function boolean(
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<boolean>,
		initial?: null | boolean,
	): null | boolean {
		let result: null | boolean = undefined!;
		let leading = 'Boolean';
		let converter = (representation: string | [string]): string | [boolean] => {
			let value: boolean;
			if (CheckHelper.is_string(representation)) {
				let check_result = CheckHelper.enumeration_checker(['n', 'y'])(representation);
				if (check_result !== null) {
					return los('console:boolean_format_error');
				}
				value = ConvertHelper.parse_boolean_from_string_of_confirmation_character(representation);
			}
			else {
				switch (representation[0]) {
					default: {
						return los('console:input_macro_invalid');
					}
				}
			}
			return [value];
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
				leading,
				() => {
					return Shell.assistant_receive_submission('boolean', []).value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ----------------

	export function integer(
		nullable: null,
		checker: null | CheckHelper.Checker<bigint>,
		initial?: bigint,
	): bigint;

	export function integer(
		nullable: boolean,
		checker: null | CheckHelper.Checker<bigint>,
		initial?: null | bigint,
	): null | bigint;

	export function integer(
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<bigint>,
		initial?: null | bigint,
	): null | bigint {
		let result: null | bigint = undefined!;
		let leading = 'Integer';
		let converter = (representation: string | [string]): string | [bigint] => {
			let value: bigint;
			if (CheckHelper.is_string(representation)) {
				let check_result = CheckHelper.regexp_checker(/^([+-])?([\d]+)$/)(representation);
				if (check_result !== null) {
					return los('console:integer_format_error', check_result);
				}
				value = BigInt(representation);
			}
			else {
				switch (representation[0]) {
					default: {
						return los('console:input_macro_invalid');
					}
				}
			}
			return [value];
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
				leading,
				() => {
					return Shell.assistant_receive_submission('integer', []).value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ----------------

	export function floater(
		nullable: null,
		checker: null | CheckHelper.Checker<number>,
		initial?: number,
	): number;

	export function floater(
		nullable: boolean,
		checker: null | CheckHelper.Checker<number>,
		initial?: null | number,
	): null | number;

	export function floater(
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<number>,
		initial?: null | number,
	): null | number {
		let result: null | number = undefined!;
		let leading = 'Floater';
		let converter = (representation: string | [string]): string | [number] => {
			let value: number;
			if (CheckHelper.is_string(representation)) {
				let check_result = CheckHelper.regexp_checker(/^([+-])?([\d]+)([.][\d]+)?$/)(representation);
				if (check_result !== null) {
					return los('console:floater_format_error', check_result);
				}
				value = Number(representation);
			}
			else {
				switch (representation[0]) {
					default: {
						return los('console:input_macro_invalid');
					}
				}
			}
			return [value];
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
				leading,
				() => {
					return Shell.assistant_receive_submission('floater', []).value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ----------------

	export function string(
		nullable: null,
		checker: null | CheckHelper.Checker<string>,
		initial?: string,
	): string;

	export function string(
		nullable: boolean,
		checker: null | CheckHelper.Checker<string>,
		initial?: null | string,
	): null | string;

	export function string(
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<string>,
		initial?: null | string,
	): null | string {
		let result: null | string = undefined!;
		let leading = 'String';
		let converter = (representation: string | [string]): string | [string] => {
			let value: string;
			if (CheckHelper.is_string(representation)) {
				value = representation;
			}
			else {
				switch (representation[0]) {
					case 'e':
					case 'empty': {
						value = '';
						break;
					}
					default: {
						return los('console:input_macro_invalid');
					}
				}
			}
			return [value];
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
				leading,
				() => {
					return Shell.assistant_receive_submission('string', []).value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ----------------

	export function size(
		nullable: null,
		checker: null | CheckHelper.Checker<StorageSize>,
		initial?: StorageSize,
	): StorageSize;

	export function size(
		nullable: boolean,
		checker: null | CheckHelper.Checker<StorageSize>,
		initial?: null | StorageSize,
	): null | StorageSize;

	export function size(
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<StorageSize>,
		initial?: null | StorageSize,
	): null | StorageSize {
		let result: null | StorageSize = undefined!;
		let leading = 'Size';
		let converter = (representation: string | [string]): string | [StorageSize] => {
			let value: StorageSize;
			if (CheckHelper.is_string(representation)) {
				let check_result = CheckHelper.regexp_checker(/^([\d]+)([.][\d]+)?([bkmg])$/)(representation);
				if (check_result !== null) {
					return los('console:size_format_error', check_result);
				}
				value = new StorageSize(representation);
			}
			else {
				switch (representation[0]) {
					default: {
						return los('console:input_macro_invalid');
					}
				}
			}
			return [value];
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
				leading,
				() => {
					return Shell.assistant_receive_submission('size', []).value;
				},
				converter,
				nullable,
				checker,
				initial,
			);
		}
		return result;
	}

	// ----------------

	export function path(
		type: 'any' | 'file' | 'directory',
		mode: 'any' | 'input' | 'output',
		nullable: null,
		checker: null | CheckHelper.Checker<StoragePath>,
		initial?: StoragePath,
	): StoragePath;

	export function path(
		type: 'any' | 'file' | 'directory',
		mode: 'any' | 'input' | 'output',
		nullable: boolean,
		checker: null | CheckHelper.Checker<StoragePath>,
		initial?: null | StoragePath,
	): null | StoragePath;

	export function path(
		type: 'any' | 'file' | 'directory',
		mode: 'any' | 'input' | 'output',
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<StoragePath>,
		initial?: null | StoragePath,
	): null | StoragePath {
		let result: null | StoragePath = undefined!;
		let leading = 'Path';
		let state_data = {
			last_value: null as null | StoragePath,
			allow_overwrite: false as boolean,
		};
		let converter = (representation: string | [string]): string | [StoragePath] => {
			let value: StoragePath;
			if (CheckHelper.is_string(representation)) {
				value = new StoragePath(ConvertHelper.unquote_string(representation));
			}
			else {
				switch (representation[0]) {
					case 'g':
					case 'generate': {
						if (mode !== 'output') {
							return los('console:path_macro_need_output');
						}
						if (state_data.last_value === null) {
							return los('console:input_macro_need_previous_input');
						}
						value = state_data.last_value;
						if (StorageHelper.exist(value)) {
							value = StorageHelper.generate_suffix_path(value, null);
							warning(los('console:path_is_exist_but_generate'), [value.emit()]);
						}
						break;
					}
					case 'm':
					case 'move': {
						if (mode !== 'output') {
							return los('console:path_macro_need_output');
						}
						if (state_data.last_value === null) {
							return los('console:input_macro_need_previous_input');
						}
						value = state_data.last_value;
						if (StorageHelper.exist(value)) {
							let move_path = StorageHelper.generate_suffix_path(value, null);
							StorageHelper.rename(value, move_path);
							warning(los('console:path_is_exist_but_move'), [move_path.emit()]);
						}
						break;
					}
					case 'd':
					case 'delete': {
						if (mode !== 'output') {
							return los('console:path_macro_need_output');
						}
						if (state_data.last_value === null) {
							return los('console:input_macro_need_previous_input');
						}
						value = state_data.last_value;
						if (StorageHelper.exist(value)) {
							StorageHelper.remove(value);
							warning(los('console:path_is_exist_but_delete'), []);
						}
						break;
					}
					case 'o':
					case 'overwrite': {
						if (mode !== 'output') {
							return los('console:path_macro_need_output');
						}
						if (state_data.last_value === null) {
							return los('console:input_macro_need_previous_input');
						}
						value = state_data.last_value;
						if (StorageHelper.exist(value)) {
							state_data.allow_overwrite = true;
							warning(los('console:path_is_exist_but_overwrite'), []);
						}
						break;
					}
					default: {
						return los('console:input_macro_invalid');
					}
				}
			}
			return [value];
		};
		let checker_proxy = (value: StoragePath): null | string => {
			state_data.last_value = value;
			if (value.type() === StoragePathType.detached) {
				return los('console:path_is_detached');
			}
			if (mode === 'input') {
				if (!StorageHelper.exist(value)) {
					return los('console:path_not_exist');
				}
				if (type === 'file' && !StorageHelper.exist_file(value)) {
					return los('console:path_is_exist_not_file');
				}
				if (type === 'directory' && !StorageHelper.exist_directory(value)) {
					return los('console:path_is_exist_not_directory');
				}
			}
			if (mode === 'output') {
				if (!state_data.allow_overwrite && StorageHelper.exist(value)) {
					return los('console:path_is_exist');
				}
			}
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
				leading,
				() => {
					return Shell.assistant_receive_submission('path', []).value;
				},
				converter,
				nullable,
				checker_proxy,
				initial,
			);
		}
		return result;
	}

	// ----------------

	export function enumeration<TValue>(
		option: Array<[TValue, string, null | string]>,
		nullable: null,
		checker: null | CheckHelper.Checker<TValue>,
		initial?: TValue,
	): TValue;

	export function enumeration<TValue>(
		option: Array<[TValue, string, null | string]>,
		nullable: boolean,
		checker: null | CheckHelper.Checker<TValue>,
		initial?: null | TValue,
	): null | TValue;

	export function enumeration<TValue>(
		option: Array<[TValue, string, null | string]>,
		nullable: null | boolean,
		checker: null | CheckHelper.Checker<TValue>,
		initial?: null | TValue,
	): null | TValue {
		let result: null | TValue = undefined!;
		let leading = 'Enumeration';
		let maximum_key_length = Math.max(...option.map((value) => (value[1].length)));
		let message = option.map((value) => (`${value[1].padStart(maximum_key_length, ' ')}${value[2] === null ? '' : `. ${value[2]}`}`));
		let converter = (representation: string | [string]): string | [TValue] => {
			let value: TValue;
			if (CheckHelper.is_string(representation)) {
				let index = -1;
				if (Shell.is_basic) {
					index = option.findIndex((item) => (item[1] === representation));
				}
				if (Shell.is_assistant) {
					index = message.findIndex((item) => (item === representation));
				}
				if (index === -1) {
					return los('console:option_invalid');
				}
				value = option[index][0];
			}
			else {
				switch (representation[0]) {
					default: {
						return los('console:input_macro_invalid');
					}
				}
			}
			return [value];
		};
		let checker_proxy = (value: TValue): null | string => {
			let index = option.findIndex((item) => (item[0] === value));
			if (index === -1) {
				return los('console:option_invalid');
			}
			return checker === null ? null : checker(value);
		};
		if (Shell.is_basic) {
			result = basic_common_input(
				leading,
				() => {
					for (let message_item of message) {
						basic_common_output(message_item, false, 1, true);
					}
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
				leading,
				() => {
					return Shell.assistant_receive_submission('enumeration', message).value;
				},
				converter,
				nullable,
				checker_proxy,
				initial,
			);
		}
		return result;
	}

	// ----------------

	export function option_boolean<TValue extends boolean>(
		value: Array<TValue>,
	): Array<[TValue, string, null | string]> {
		return value.map((value, index) => ([value, `${ConvertHelper.make_boolean_to_string_of_confirmation_character(value)}`, `${ConvertHelper.make_boolean_to_string(value)}`]));
	}

	export function option_integer<TValue extends bigint>(
		value: Array<TValue>,
	): Array<[TValue, string, null | string]> {
		return value.map((value, index) => ([value, `${String.fromCharCode(index + 'a'.charCodeAt(0))}`, `${ConvertHelper.make_integer_to_string(value)}`]));
	}

	export function option_floater<TValue extends number>(
		value: Array<TValue>,
	): Array<[TValue, string, null | string]> {
		return value.map((value, index) => ([value, `${String.fromCharCode(index + 'a'.charCodeAt(0))}`, `${ConvertHelper.make_floater_to_string(value)}`]));
	}

	export function option_string<TValue extends string>(
		value: Array<TValue>,
	): Array<[TValue, string, null | string]> {
		return value.map((value, index) => ([value, `${index + 1}`, `${value}`]));
	}

	export function option_size<TValue extends StorageSize>(
		value: Array<TValue>,
	): Array<[TValue, string, null | string]> {
		return value.map((value, index) => ([value, `${index + 1}`, `${value.emit()}`]));
	}

	export function option_path<TValue extends StoragePath>(
		value: Array<TValue>,
	): Array<[TValue, string, null | string]> {
		return value.map((value, index) => ([value, `${index + 1}`, `${value.emit()}`]));
	}

	// #endregion

	// #region advanced feature

	export function pick_storage_item(
		type: null | 'load_file' | 'load_directory' | 'save_file',
	): null | StoragePath {
		let result: null | StoragePath = undefined!;
		if (type === null) {
			information(los('console:pick_path_type'), []);
			type = enumeration(option_string(['load_file', 'load_directory', 'save_file']), null, null) as 'load_file' | 'load_directory' | 'save_file';
		}
		if (Shell.is_basic) {
			// unavailable, silently fail
			result = null;
		}
		if (Shell.is_assistant) {
			let result_value = Shell.assistant_pick_storage_item(type).target;
			if (result_value !== '') {
				result = new StoragePath(result_value);
			}
		}
		return result;
	}

	export function push_system_notification(
		title: string,
		description: string,
	): void {
		if (Shell.is_basic) {
			// unavailable, silently fail
		}
		if (Shell.is_assistant) {
			Shell.assistant_push_system_notification(title, description);
		}
		return;
	}

	// #endregion

}
