namespace TwinStar.Script.Executor {

	// ------------------------------------------------

	export type Command = {
		/** 需要应用的功能，若未指定，则需由用户选择 */
		method: string | null;
		/** 传递给所应用功能工作函数的参数 */
		argument: Record<string, Core.JSON.JS_Value>;
		/** 附加输入 */
		input: null | {
			/** 输入值 */
			value: string;
			/** 关闭功能的过滤功能；若指定，则（在未指定建议时）在筛选可用功能时不使用过滤功能，所有功能都会进入待选表 */
			disable_filter: boolean;
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
						disable_filter: false,
					};
					if (index < raw_command.length && raw_command[index] === '-disable_filter') {
						++index;
						command.input.disable_filter = true;
					}
				}
			}
			if (index < raw_command.length && raw_command[index] === '-method') {
				++index;
				command.method = raw_command[index++];
			}
			if (index < raw_command.length && raw_command[index] === '-argument') {
				++index;
				let argument = CoreX.JSON.read_s_js(raw_command[index++]);
				assert_test(argument !== null && typeof argument === 'object' && (argument as Object).constructor.name === 'Object' && !(argument instanceof Array), `argument must be a object`);
				command.argument = argument;
			}
			result.push(command);
		}
		return result;
	}

	export function execute(
		command: Command,
		method: Array<Method>,
	): void {
		let selected_method: Method | null;
		if (command.method !== null) {
			let target_method = method.find((e) => (e.id === command.method));
			assert_test(target_method !== undefined, `invalid method id`);
			selected_method = target_method;
		} else {
			if (command.input === null) {
				Console.message('i', los('executor.command:no_input'), [
				]);
				let input_value = Console.string(null, null);
				Console.message('i', los('executor.command:should_disable_filter_or_not'), [
				]);
				let input_disable_filter = Console.confirmation(null, null);
				command.input = {
					value: input_value,
					disable_filter: input_disable_filter,
				};
			}
			let method_state: Array<boolean> = [];
			for (let i in method) {
				method_state[i] = command.input.disable_filter || method[i].input_filter(command.input.value);
			}
			if (!method_state.includes(true)) {
				Console.message('w', los('executor.command:no_method_so_pass'), [
				]);
				selected_method = null;
			} else {
				Console.message('i', los('executor.command:select_method'), [
					los('executor.command:input_null_to_pass'),
				]);
				selected_method = Console.option(method_state.map((e, i) => (e ? [method[i], `${method[i].name()}`] : null)), true, null);
			}
		}
		if (selected_method !== null) {
			let argument = { ...command.argument };
			if (command.input !== null) {
				argument[selected_method.input_forwarder] = command.input.value;
			}
			for (let key in selected_method.default_argument) {
				if (argument[key] === undefined) {
					assert_test(selected_method.default_argument[key] !== undefined, `argument <${key}> is required`);
					argument[key] = selected_method.default_argument[key];
				}
			}
			selected_method.worker(argument);
		}
		return;
	}

	// ------------------------------------------------

}