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
		/** 需要应用的功能，若未指定，则需由用户选择 */
		method: null | string;
		/** 传递给所应用功能工作函数的参数 */
		argument: Argument;
		/** 附加输入 */
		input: null | {
			/** 输入值 */
			value: string;
			/** 关闭功能的过滤功能；若指定，则（在未指定建议时）在筛选可用功能时不使用过滤功能，所有功能都会进入待选表 */
			filterless: boolean;
		};
	};

	// ------------------------------------------------

	export function parse(
		raw_command: Array<string>,
	): Array<Command> {
		let result: Array<Command> = [];
		let index = 0;
		while (index < raw_command.length) {
			let command: Command = {
				method: null,
				argument: {},
				input: null,
			};
			{
				let input_value = raw_command[index++];
				if (input_value !== '?') {
					command.input = {
						value: input_value,
						filterless: false,
					};
					if (index < raw_command.length && raw_command[index] === '-filterless') {
						index++;
						command.input.filterless = true;
					}
				}
			}
			if (index < raw_command.length && raw_command[index] === '-method') {
				index++;
				command.method = raw_command[index++];
			}
			if (index < raw_command.length && raw_command[index] === '-argument') {
				index++;
				let argument = KernelX.JSON.read_s_js(raw_command[index++]);
				assert_test(is_object_of_object(argument), `argument must be a object`);
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
		let selected_method: null | Method;
		if (command.method !== null) {
			let target_method = method.find((value) => (value.id === command.method));
			if (target_method === undefined) {
				target_method = fallback_method.find((value) => (value.id === command.method));
			}
			if (target_method === undefined) {
				selected_method = null;
				state = los('executor.generic:method_invalid');
			}
			else {
				selected_method = target_method;
			}
		}
		else {
			if (command.input === null) {
				Console.information(los('executor.generic:no_input'), [
				]);
				let input_value = Console.string(null, null);
				Console.information(los('executor.generic:should_filterless_or_not'), [
				]);
				let input_filterless = Console.boolean(null, null);
				command.input = {
					value: input_value,
					filterless: input_filterless,
				};
			}
			let valid_method: Array<[bigint, Method]> = [];
			for (let index in method) {
				if (command.input.filterless || method[index].input_filter(command.input.value)) {
					valid_method.push([BigInt(index) + 1n, method[index]]);
				}
			}
			if (valid_method.length === 0) {
				selected_method = null;
				state = los('executor.generic:method_unavailable');
			}
			else {
				Console.information(los('executor.generic:method_select'), [
					los('executor.generic:method_select_null_to_skip'),
				]);
				selected_method = Console.enumeration(valid_method.map((value) => ([value[1], `${value[0]}`, value[1].name()])), true, null);
				if (selected_method === null) {
					state = los('executor.generic:method_select_null');
				}
			}
			if (selected_method === null) {
				valid_method = [];
				for (let index in fallback_method) {
					if (command.input.filterless || fallback_method[index].input_filter(command.input.value)) {
						valid_method.push([BigInt(index) + 1n, fallback_method[index]]);
					}
				}
				if (valid_method.length !== 0) {
					Console.information(los('executor.generic:method_select'), [
						los('executor.generic:method_select_null_to_skip'),
					]);
					selected_method = Console.enumeration(valid_method.map((value) => ([value[1], `${value[0]}`, value[1].name()])), true, null);
					if (selected_method !== null) {
						state = undefined!;
					}
				}
			}
		}
		if (selected_method !== null) {
			let argument = { ...command.argument };
			if (command.input !== null) {
				argument[selected_method.input_forwarder] = command.input.value;
			}
			for (let key in selected_method.default_argument) {
				if (argument[key] === undefined) {
					argument[key] = selected_method.default_argument[key];
				}
			}
			state = selected_method.worker(argument);
		}
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