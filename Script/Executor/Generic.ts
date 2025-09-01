namespace Twinning.Script.Executor {

	// ------------------------------------------------

	export type Argument = Record<string, null | boolean | bigint | number | string>;

	export type Method<Argument extends Executor.Argument = Executor.Argument, InputForwarderKey extends keyof Argument = keyof Argument> = {
		/** ID；用于指定功能，应唯一 */
		id: string;
		/** 名称函数；返回对函数的名称文本 */
		name(): string;
		/** 工作函数；根据参数执行的事务，返回结束状态与有效用时 */
		worker(argument: Argument): [boolean, number];
		/** 默认参数；当命令所提供参数未包含所需参数时，提供默认值，若其中属性值为undefined，则表示命令所提供参数必须包含此值 */
		default_argument: Argument;
		/** 输入过滤函数；根据输入值判断此功能是否可进入待选表 */
		input_filter(input: string): boolean;
		/** 输入转发键名；输入值需要转发到工作函数参数的键名 */
		input_forwarder: InputForwarderKey;
	};

	export type Command = {
		/** 附加输入值 */
		input: null | string;
		/** 关闭功能的筛选；若指定，则所有功能都可供用户选择 */
		filterless: boolean;
		/** 需要应用的功能，若未指定，则需由用户选择 */
		method: null | string;
		/** 传递给所应用功能工作函数的参数 */
		argument: Argument;
	};

	// ------------------------------------------------

	export function parse(
		command_line: Array<string>,
	): Array<Command> {
		let result: Array<Command> = [];
		let command_reader = new CommandLineReader(command_line);
		while (!command_reader.done()) {
			let command: Command = {
				input: null,
				filterless: false,
				method: null,
				argument: {},
			};
			{
				let input = command_reader.next_string();
				command.input = input === '?' ? null : input;
			}
			if (command_reader.check('-filterless')) {
				command.filterless = true;
			}
			if (command_reader.check('-method')) {
				command.method = command_reader.next_string();
			}
			if (command_reader.check('-argument')) {
				let argument = KernelX.JSON.read_s_js(command_reader.next_string());
				if (!is_object_of_object(argument)) {
					throw new Error(`argument must be a object`);
				}
				command.argument = argument as Argument;
			}
			result.push(command);
		}
		return result;
	}

	export function execute(
		command: Command,
		method: Array<Method>,
		fallback_method: Array<Method>,
	): null | [boolean, number] {
		let state: [boolean, number] | string = undefined!;
		do {
			if (command.method === null) {
				if (command.input === null) {
					state = los('executor.generic:input_required');
					break;
				}
				for (let method_source of [method, fallback_method]) {
					let valid_method: Array<[bigint, Method]> = [];
					for (let index in method_source) {
						if (command.filterless || method_source[index].input_filter(command.input)) {
							valid_method.push([BigInt(index) + 1n, method_source[index]]);
						}
					}
					if (valid_method.length !== 0) {
						Console.information(los('executor.generic:method_select'), [
							los('executor.generic:method_select_null_to_skip'),
						]);
						command.method = Console.enumeration(valid_method.map((value) => ([value[1].id, `${value[0]}`, value[1].name()])), true, null);
						if (command.method !== null) {
							break;
						}
					}
				}
			}
			if (command.method === null) {
				state = los('executor.generic:method_unavailable');
				break;
			}
			let target_method = method.find((value) => (value.id === command.method));
			if (target_method === undefined) {
				target_method = fallback_method.find((value) => (value.id === command.method));
			}
			if (target_method === undefined) {
				state = los('executor.generic:method_invalid');
				break;
			}
			let argument = { ...command.argument };
			if (command.input !== null) {
				argument[target_method.input_forwarder] = command.input;
			}
			for (let key in target_method.default_argument) {
				if (argument[key] === undefined) {
					argument[key] = target_method.default_argument[key];
				}
			}
			state = target_method.worker(argument);
		}
		while (false);
		if (is_string(state)) {
			Console.warning(los('executor.generic:finish_skipped'), [state]);
		}
		else {
			if (state[0] === false) {
				Console.error(los('executor.generic:finish_failed'), [los('executor.generic:duration', (state[1] / 1000).toFixed(3))]);
			}
			if (state[0] === true) {
				Console.success(los('executor.generic:finish_succeeded'), [los('executor.generic:duration', (state[1] / 1000).toFixed(3))]);
			}
		}
		return is_string(state) ? null : state;
	}

	// ------------------------------------------------

	export const g_method: Array<Executor.Method> = [];

	export const g_method_batch: Array<Executor.Method> = [];

	// ------------------------------------------------

}